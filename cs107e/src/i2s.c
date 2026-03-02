/*
 * File: i2s.c
 *
 * Description: Contains API support for I2S devices on the MangoPi. Allows users
 * to initialize the I2S module. Check i2c.h for more specification on the
 * `i2s_frame_type_t` and `i2s_freq_t` types.
 *
 * Author: Chris Gregg     <cgregg@stanford.edu>
 * Author: Julie Zelenski  <zelenski@cs.stanford.edu>
 * Author: Joe Robertazzi  <tazzi@stanford.edu>
 *
 * Last updated: Curis summer 2025
 */

#include "i2s.h"
#include "assert.h"
#include "ccu.h"
#include "dma.h"
#include "gpio.h"
#include "printf.h"

// Bit codes for selecting sample resolution (formula = (res >> 2) - 1 in binary)
typedef enum {
    SAMPLE_RES_16 = 0b011,
    SAMPLE_RES_24 = 0b101,
} i2s_sample_resolution;

// Allows the driver to track the progress of non-blocking transfers.
typedef struct {
    dma_transfer_id_t id;
} i2s_transfer_t;

// Structs defined to match layout of hardware registers
typedef union {
    struct {
        struct {
            uint32_t global_ena : 1;
            uint32_t rx_ena     : 1;
            uint32_t tx_ena     : 1;
            uint32_t            : 1;
            uint32_t mode       : 2;
            uint32_t out_mute   : 1;
            uint32_t            : 1;
            uint32_t dout_ena   : 4; // dout0-3
            uint32_t            : 5;
            uint32_t dir_bclk   : 1;
            uint32_t dir_lrclk  : 1;
            uint32_t            :13;
        } ctl;
        struct {
            uint32_t slot_width : 3;
            uint32_t            : 1;
            uint32_t sample_res : 3;
            uint32_t            : 1;
            uint32_t lr_period  :10;
            uint32_t            :13;
            uint32_t            :32; // fmt1 fields
        } fmt;
        uint32_t ista;
        const uint32_t rx_fifo;
        struct {
            uint32_t rx_mode    : 2;
            uint32_t tx_mode    : 1;
            uint32_t            : 1;
            uint32_t rx_trigger : 6;
            uint32_t            : 2;
            uint32_t tx_trigger : 6;
            uint32_t            : 5;
            uint32_t rx_flush   : 1;
            uint32_t tx_flush   : 1;
            uint32_t            : 7;
        } fifoctl;
        const struct {
            uint32_t rx_acnt    : 7;
            uint32_t            : 1;
            uint32_t rx_avail   : 1;
            uint32_t            : 7;
            uint32_t tx_ecnt    : 8;
            uint32_t            : 4;
            uint32_t tx_empty   : 1;
            uint32_t            : 3;
        } fsta;
        struct uint32_t {
            uint32_t rxai_en    : 1;
            uint32_t rxoi_en    : 1;
            uint32_t rxui_en    : 1;
            uint32_t rx_drq     : 1;
            uint32_t txei_en    : 1;
            uint32_t txoi_en    : 1;
            uint32_t txui_en    : 1;
            uint32_t tx_drq     : 1;
            uint32_t            :24;
        } irq;
        uint32_t tx_fifo;
        struct {
            uint32_t mclk_div   : 4;
            uint32_t bclk_div   : 3;
            uint32_t mclk_ena   : 1;
            uint32_t            :24;
        } clkd;
        uint32_t tx_cntr;
        uint32_t rx_cntr;
        struct {
            uint32_t tx_slot_num : 4;
            uint32_t rx_slot_num : 4;
            uint32_t             :24;
        } chcfg;
        struct {
            uint32_t chen       :16;
            uint32_t chsel      : 4;
            uint32_t offset     : 2;
            uint32_t            :10;
        } txchsel[4];
        struct {
            uint32_t            :32; // padding for channels 8 - 15
            uint32_t ch0        : 4; // since we only use mono/stereo, we only need channels 1/0
            uint32_t ch1        : 4; // can manually change the other fields if need more channels
            uint32_t            :24; // padding for channels 2 - 7
        } txchmap[4];
        struct {
            uint32_t            :16;
            uint32_t chsel      : 4;
            uint32_t offset     : 2;
            uint32_t            :10;
        } rxchsel;
        struct {
           uint32_t             :32; // padding for channels 12 - 15
           uint32_t             :32; // padding for channels 8 - 11
           uint32_t             :32; // padding for channels 4 - 7
           uint32_t ch0map      : 4;
           uint32_t ch0sel      : 2;
           uint32_t             : 2;
           uint32_t ch1map      : 4;
           uint32_t ch1sel      : 2;
           uint32_t             : 2;
           uint32_t             :16; // padding for channels 2 - 3
        } rxchmap;
    } regs;
    unsigned char padding[0x1000]; // sizeof entire I2S block
} i2s_t;

// DO NOT REMOVE - confirms that our bit field is laid out corrected
#define I2S_BASE ((i2s_t *)0x02032000)
_Static_assert(&(I2S_BASE[0].regs.tx_fifo)  == (void *)0x02032020, "i2s0 ttx_fifo reg must be at address 0x02032040");
_Static_assert(&(I2S_BASE[1].regs.rxchsel)  == (void *)0x02033064, "i2s1 rxchsel must be at address 0x02033000");
_Static_assert(&(I2S_BASE[2].regs.ctl)      == (void *)0x02034000, "i2s2 ctrl reg must be at address 0x02032080");
_Static_assert(sizeof(I2S_BASE[2].regs.rxchmap) == 16, "i2s2 rxchmap should have one byte per channel = 16 bytes total");

// Struct containing i2s config data
static struct {
    volatile i2s_t * const i2s_base, *i2s;
    long mclk_rate;
    int frequency;
    bool is_active;
    i2s_transfer_t current_transfer;
} module = {
    .i2s_base = &I2S_BASE[0],
    .i2s = NULL, // points to i2s after init()
    .is_active = false,
};

// Struct containing audio clock configuration
static struct audio_clock {
    uint32_t multiple; // clock setting appropriate for output frequency of this multiple
    struct {
        ccu_pll_id_t id;
        long rate;
    } pll;
    struct {
        ccu_parent_id_t parent;
        long rate, actual_rate;
    } mod_clk;
} clock_options [] = {
    {.multiple= 11025, .pll= {.id=CCU_PLL_AUDIO0_CTRL_REG, .rate= 22545454},   .mod_clk = {.parent= PARENT_AUDIO0, .rate= 22545454}},
    {.multiple=  8000, .pll= {.id=CCU_PLL_AUDIO1_CTRL_REG, .rate= 3072000000}, .mod_clk = {.parent= PARENT_AUDIO1_DIV5, .rate= 24576000}},
    {0},
};

// Helper function prototypes
static void validate_request(void);
static void choose_clock(int sample_frequency);
static void set_clock_divider(int bitrate);
static void config_gpio(void);
static void config_for_playback(i2s_frame_type_t ftype);
static void config_for_capture(i2s_frame_type_t ftype);
static void playback_start(void);
static void capture_start(void);

// Initializes the I2S module -- must be called before calling any other I2S operations
void i2s_init(i2s_freq_t sample_frequency) {
     // disbale before config (reset in case multiple inits in one program)
    module.i2s->regs.ctl.global_ena = 0;

    module.i2s = &I2S_BASE[2]; // use I2S2, gpio pins on header
    ccu_ungate_bus_clock_bits(CCU_I2S_BGR_REG, 1 << 18, 1 << 2); // I2S2
    choose_clock(sample_frequency); // configure clocks
    int bits_per_sample             = 32;
    int bits_per_frame = bits_per_sample * 2;
    int bitrate = module.frequency * bits_per_frame;
    set_clock_divider(bitrate);
    module.i2s->regs.fmt.lr_period  = 31;
    module.i2s->regs.ctl.global_ena = 0;
    module.i2s->regs.ctl.rx_ena     = 0;
    module.i2s->regs.ctl.tx_ena     = 0;
    config_gpio();

    module.i2s->regs.ctl.dir_bclk   = 1;      // BCLK direction output
    module.i2s->regs.ctl.dir_lrclk  = 1;      // LRCLK direction output
    module.i2s->regs.clkd.mclk_ena  = 0;      // disable MCLK (not used)
    module.i2s->regs.ctl.mode       = 1;      // mode is I2S standard, left-justified
    module.i2s->regs.ctl.dout_ena   = 0b0001; // enable D0, disable D1-3

    // enable after config
    module.i2s->regs.ctl.global_ena = 1;
}

// Returns whether the current i2s transaction has completed
bool i2s_transfer_completed(void) {
    if (!module.is_active) error("must call a playback/capture before calling i2s_transfer_completed()\n");
    return dma_transfer_completed(module.current_transfer.id);
}

// Blocks until the current I2S transaction has completed
void i2s_await(void) {
    if (!module.is_active) error("must call a playback/capture before calling i2s_await()\n");
    while (!dma_transfer_completed(module.current_transfer.id)) {}
}

// Non-blocking DMA implementation for streaming playback using the I2S device
void i2s_stream_playback_nb(const int16_t samples[], int nsamples, i2s_frame_type_t ftype) {
    validate_request();

    config_for_playback(ftype);
    playback_start();
    module.i2s->regs.irq.tx_drq = 1;

    dma_endpoint_t from_mem         = dma_create_endpoint(DRQ_TYPE_DRAM, samples);
    dma_endpoint_t to_i2s_tx        = dma_create_endpoint(DRQ_TYPE_I2S2, &module.i2s->regs.tx_fifo);
    module.current_transfer.id      = dma_transfer(from_mem, to_i2s_tx, DMA_BITWIDTH_16, nsamples * sizeof(*samples));
}

// Non-blocking DMA implementation for capturing samples from the I2S device
void i2s_stream_capture_nb(int16_t samples[], int nsamples, i2s_frame_type_t ftype) {
    validate_request();

    config_for_capture(ftype);
    capture_start();
    module.i2s->regs.irq.rx_drq = 1;

    dma_endpoint_t from_i2s_rx      = dma_create_endpoint(DRQ_TYPE_I2S2, (uint16_t*)((char*)(&(module.i2s->regs.rx_fifo)) + 2));
    dma_endpoint_t to_mem           = dma_create_endpoint(DRQ_TYPE_DRAM, samples);
    module.current_transfer.id      = dma_transfer(from_i2s_rx, to_mem, DMA_BITWIDTH_16, nsamples * sizeof(*samples));
}

// Validates whether I2S has been properly initialized and if the previous transfer has completed already
static void validate_request(void) {
    if (module.i2s == NULL) error("i2s_init() has not been called!\n");
    if (module.is_active) {
        if (!dma_transfer_completed(module.current_transfer.id)) {
            error("attempted new I2S transfer while previous transfer still underway.\n"
                  "hint: use i2s_await() to wait for your previous non-blocking transfer to complete."
                );
        }
    }
    module.is_active = true; // Ensures that we don't check module.current_transfer before it's initialized
}

// Chooses the appropriate clock based on the input sample frequency
static void choose_clock(int sample_frequency) {
    for (struct audio_clock *opt = clock_options; opt->multiple; opt++) {
        if (sample_frequency % opt->multiple == 0) {
            if (opt->mod_clk.actual_rate == 0) ccu_config_pll_rate(opt->pll.id, opt->pll.rate); // only config pll once
            opt->mod_clk.actual_rate = ccu_config_module_clock_rate(CCU_I2S2_CLK_REG, opt->mod_clk.parent, opt->mod_clk.rate);
            assert(opt->mod_clk.actual_rate == opt->mod_clk.rate); // confirm we have rate we wanted
            module.frequency = sample_frequency;
            module.mclk_rate = opt->mod_clk.actual_rate;
            return;
        }
    }
    assert(!"No available clock option for requested frequency");
}

// Sets the clock divider based on the input bitrate
static void set_clock_divider(int bitrate) {
    int fraction = module.mclk_rate/bitrate;
    const int available_dividers[] = { -1, 1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 176, 192 };
    int n_avail = sizeof(available_dividers)/sizeof(*available_dividers);
    int chosen_index = -1;
    for (int i = 0; i < n_avail; i++) {
        if (available_dividers[i] >= fraction) {
            chosen_index = i; break;
        }
    }
    module.i2s->regs.clkd.bclk_div = chosen_index;
}

// Configures GPIO pins for I2S
static void config_gpio(void) {
    static struct  {
        gpio_id_t pin;
        uint8_t fn;                         // Connections from Mango Pi to pins on DAC
    } bclk  = {GPIO_PB5, GPIO_FN_ALT3},     // GPIO_PB5 to BCLK
      lrclk = {GPIO_PB6, GPIO_FN_ALT3},     // GPIO_PB6 to LRCLK
      dout0 = {GPIO_PB4, GPIO_FN_ALT3},     // GPIO_PB4 to Data Out
      din0  = {GPIO_PB3, GPIO_FN_ALT5};     // GPIO_PB3 to Data In
    gpio_set_function(bclk.pin,  bclk.fn);
    gpio_set_function(lrclk.pin, lrclk.fn);
    gpio_set_function(dout0.pin, dout0.fn);
    gpio_set_function(din0.pin, din0.fn);
}

// Configures I2S registers for playback
static void config_for_playback(i2s_frame_type_t ftype) {
    int bits_per_sample = 16;                                // hardcoded at 16 bits since we're using 16-bit samples
    int bits_per_frame = bits_per_sample * 2;
    int bitrate = module.frequency * bits_per_frame;
    set_clock_divider(bitrate);
    module.i2s->regs.fmt.lr_period = bits_per_sample - 1;    // bclks per sample
    module.i2s->regs.fmt.sample_res = bits_per_sample/4 - 1; // sample resolution = 16
    module.i2s->regs.fmt.slot_width = bits_per_sample/4 - 1; // slot width = 16
    module.i2s->regs.txchsel[0].offset = 1;                  // mode is i2s standard: data offset by 1 BCLK relative to LRCK
    module.i2s->regs.fifoctl.tx_mode = 1;                    // if sample width < 32-bit, harvest from LSB of fifo

    int n_output_channels = 2;                               // output 2-channel (either stereo or duplicate mono)
    uint32_t enable_bits = (1 << n_output_channels) - 1;
    module.i2s->regs.txchsel[0].chen = enable_bits;
    module.i2s->regs.txchsel[0].chsel = n_output_channels - 1; // set size of map

    // mono frame consumes 1 sample from fifo
    // stereo frame consumes 2 samples from fifo
    module.i2s->regs.chcfg.tx_slot_num = ftype == I2S_MONO ? 0 : 1;
    module.i2s->regs.txchmap[0].ch0 = 0;                             // fill L => first sample
    module.i2s->regs.txchmap[0].ch1 = ftype == I2S_MONO ? 0 : 1;     // fill R => first sample (replicate if ftype = I2S_MONO)
                                                                     // fill R => second sample (if ftype = I2S_STEREO)
}

// Configures I2S registers for capture
static void config_for_capture(i2s_frame_type_t ftype) {
    int bits_per_sample = ftype == I2S_MONO ? 16 : 32;          // 16 bits for mono, 32 bits for stereo
    int bits_per_frame = bits_per_sample * 2;
    int bitrate = module.frequency * bits_per_frame;
    set_clock_divider(bitrate);
    module.i2s->regs.fmt.lr_period = 31;             // bclks per sample
    module.i2s->regs.fmt.sample_res = SAMPLE_RES_16; // 16-bit sample resolution
    module.i2s->regs.fmt.slot_width = SAMPLE_RES_16; // 16-bit slot width
    module.i2s->regs.fifoctl.rx_mode = 0;            // if sample width < 32-bit, zero fill
    module.i2s->regs.rxchsel.chsel = 1;              // two input channels (right will duplicate left)
    module.i2s->regs.rxchsel.offset = 1;             // mode is i2s standard: data offset by 1 BCLK relative to LRCK
    int n_samples_fifo = 2;
    module.i2s->regs.chcfg.rx_slot_num = n_samples_fifo - 1;
    module.i2s->regs.rxchmap.ch0map = 0;                           // left sample
    module.i2s->regs.rxchmap.ch1map = ftype == I2S_MONO ? 0 : 1;   // right --> duplicate if ftype = I2S_MONO
                                                                   // use second channel if ftype = I2S_STEREO
}

// Starts audio playback on the I2S device
static void playback_start(void) {
    module.i2s->regs.fifoctl.tx_flush = 1;
    module.i2s->regs.tx_cntr = 0;
    module.i2s->regs.ctl.tx_ena = 1;
}

//Starts capturing samples from the I2S device
static void capture_start(void) {
    module.i2s->regs.fifoctl.rx_flush = 1;
    module.i2s->regs.rx_cntr = 0;
    module.i2s->regs.ctl.rx_ena = 1;
}


#if 0
/* Example code for blocking implementation for I2S playback (not utilizing the DMA). Use this
 * code as a template if you need access to samples as they enter the fifo and exit the samples[] array.
 * This code manually loops through the fifo one element at a time so it prevent you from being able to
 * run tasks in the background, but gives you the ability to directly interact with the fifo stream
 * in real time.
 */
void i2s_play_stream_blocking(const int16_t samples[], int nsamples, i2s_frame_type_t ftype) {
    if (module.i2s == NULL) error("i2s_init() has not been called!\n");
    config_for_playback(ftype);
    playback_start();
    for (int i = 0; i < nsamples; i++) {
        while (module.i2s->regs.fsta.tx_ecnt == 0) ; // wait for empty space in fifo
        module.i2s->regs.tx_fifo = samples[i];
    }
    module.i2s->regs.ctl.tx_ena = 0; // done audio playback

}

/* Example code for blocking implementation for I2S capture (not utilizing the DMA). Use this code
 * as a template if you need access to samples as they exit the fifo and enter the samples[] array.
 * This code manually loops through the fifo one element at a time so it prevent you from being able to
 * run tasks in the background, but gives you the ability to directly interact with the fifo stream
 * in real time.
 */
void i2s_capture_blocking(int16_t samples[], int nsamples) {
    if (module.i2s == NULL) error("i2s_init() has not been called!\n");
    config_for_capture(I2S_MONO);
    capture_start();
    for (int i = 0; i < nsamples; i++) {
        while (module.i2s->regs.fsta.rx_acnt == 0) ; // wait for sample avail
        uint32_t val = module.i2s->regs.rx_fifo;
        samples[i] = val >> 16; // Most you can shift by before distortion is 12 bits
    }
    module.i2s->regs.ctl.rx_ena = 0; // done audio capture
}

#endif