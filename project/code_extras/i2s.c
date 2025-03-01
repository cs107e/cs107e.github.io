/*
 * Module to interface with I2S hardware peripheral
 *
 * Author: Chris Gregg <cgregg@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Wed Nov 20 18:46:12 PST 2024
 */

#include "i2s.h"
#include "assert.h"
#include "ccu.h"
#include "gpio.h"
#include <stdint.h>
#include "printf.h"

// structs defined to match layout of hardware registers

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
            uint32_t            : 13;
        } ctl;
        struct {
            uint32_t slot_width : 3;
            uint32_t            : 1;
            uint32_t sample_res : 3;
            uint32_t            : 1;
            uint32_t lr_period  : 10;
            uint32_t            : 13;
            uint32_t            : 32; // fmt1 fields
        } fmt;
        uint32_t ista;
        uint32_t rx_fifo;
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
        struct {
            uint32_t rx_acnt    : 7;
            uint32_t            : 1;
            uint32_t rx_avail   : 1;
            uint32_t            : 7;
            uint32_t tx_ecnt    : 8;
            uint32_t            : 4;
            uint32_t tx_empty   : 1;
            uint32_t            : 3;
        } fsta;
        uint32_t irq;
        uint32_t tx_fifo;
        struct {
            uint32_t            : 4;
            uint32_t bclk_div   : 3;
            uint32_t mclk_ena   : 1;
            uint32_t            : 24;
        } clkd;
        uint32_t tx_cntr;
        uint32_t rx_cntr;
        struct {
            uint32_t tx_slot_num : 4;
            uint32_t rx_slot_num : 4;
            uint32_t             : 24;
        } chcfg;
        struct {
            uint32_t chen       : 16;
            uint32_t chsel      : 4;
            uint32_t offset     : 2;
            uint32_t            : 10;
        } txchsel[4];
        struct {
            uint32_t ch8        : 4;
            uint32_t ch9        : 4;
            uint32_t ch10       : 4;
            uint32_t ch11       : 4;
            uint32_t ch12       : 4;
            uint32_t ch13       : 4;
            uint32_t ch14       : 4;
            uint32_t ch15       : 4;
            uint32_t ch0        : 4;
            uint32_t ch1        : 4;
            uint32_t ch2        : 4;
            uint32_t ch3        : 4;
            uint32_t ch4        : 4;
            uint32_t ch5        : 4;
            uint32_t ch6        : 4;
            uint32_t ch7        : 4;
        } txchmap[4];
        uint32_t rxchsel;
        uint32_t rxchhmap[4];
    } regs;
    unsigned char padding[0x1000]; // sizeof entire I2S block
} i2s_t;

#define I2S_BASE ((i2s_t *)0x02032000)
_Static_assert(&(I2S_BASE[0].regs.tx_fifo)  == (void *)0x02032020, "i2s0 ttx_fifo reg must be at address 0x02032040");
_Static_assert(&(I2S_BASE[1].regs.rxchsel)  == (void *)0x02033064, "i2s1 rxchsel must be at address 0x02033000");
_Static_assert(&(I2S_BASE[2].regs.ctl)      == (void *)0x02034000, "i2s2 ctrl reg must be at address 0x02032080");

static volatile i2s_t * const module = &I2S_BASE[2]; // use i2s2, has gpio pinouts on header

static struct audio_clock {
    uint32_t multiple; // clock setting appropriate for output frequency of this multiple
    struct {
        ccu_pll_id_t id;
        long rate;
    } pll;
    struct {
        ccu_parent_id_t parent;
        long rate, actual_rate;
    } module;
} clock_options [] = {
    {.multiple= 11025, .pll= {.id=CCU_PLL_AUDIO0_CTRL_REG, .rate= 22545454},   .module= {.parent= PARENT_AUDIO0, .rate= 22545454}},
    {.multiple=  8000, .pll= {.id=CCU_PLL_AUDIO1_CTRL_REG, .rate= 3072000000}, .module= {.parent= PARENT_AUDIO1_DIV5, .rate= 24576000}},
    {0},
};

static long choose_clock(int sample_frequency) {
    for (struct audio_clock *opt = clock_options; opt->multiple; opt++) {
        if (sample_frequency % opt->multiple == 0) {
            if (opt->module.actual_rate == 0) ccu_config_pll_rate(opt->pll.id, opt->pll.rate); // only config pll once
            opt->module.actual_rate = ccu_config_module_clock_rate(CCU_I2S2_CLK_REG, opt->module.parent, opt->module.rate);
            assert(opt->module.actual_rate == opt->module.rate); // confirm we have rate we wanted
            ccu_ungate_bus_clock_bits(CCU_I2S_BGR_REG, 1 << 18, 1 << 2);
            return opt->module.actual_rate;
        }
    }
    assert(!"No available clock option for requested frequency");
    return 0;
}

static void config_gpio(void) {
    static struct  {
        gpio_id_t pin;
        uint8_t fn;        // Connections from Mango Pi to pins on DAC
    } bclk =  {GPIO_PB5, GPIO_FN_ALT3},     // GPIO_PB5 to BCLK
      lrclk = {GPIO_PB6, GPIO_FN_ALT3},     // GPIO_PB6 to LRCLK
      dout0 = {GPIO_PB4, GPIO_FN_ALT3};     // GPIO_PB4 to Data In
    gpio_set_function(bclk.pin,  bclk.fn);
    gpio_set_function(lrclk.pin, lrclk.fn);
    gpio_set_function(dout0.pin, dout0.fn);
}

void i2s_init(int sample_frequency) {
    config_gpio();
    long mclk_rate = choose_clock(sample_frequency); // configure clocks

    module->regs.ctl.global_ena = 0;
    module->regs.ctl.rx_ena = 0;
    module->regs.ctl.tx_ena = 0;

    module->regs.fifoctl.rx_flush = 0;
    module->regs.fifoctl.tx_flush = 0;
    module->regs.tx_cntr = 0;
    module->regs.rx_cntr = 0;

    module->regs.ctl.dir_bclk = 1;      // BCLK direction output
    module->regs.ctl.dir_lrclk = 1;     // LRCLK direction output
    module->regs.clkd.mclk_ena = 0;     // disable MCLK (not used)
    module->regs.ctl.mode = 1;          // mode is I2S standard, left-justified
    module->regs.ctl.dout_ena = 0b0001; // enable D0, disable D1-3
    module->regs.ctl.out_mute = 1;      // initially mute output

    int bits_per_sample = 16; //  DAC I'm using supports 16 bps (other bps not tested/confirmed)
    int bitrate = sample_frequency * bits_per_sample * 2; // L+R channels in each sample
    int fraction = mclk_rate/bitrate;
    const int available_dividers[] = { -1, 1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 176, 192 };
    int n_avail = sizeof(available_dividers)/sizeof(*available_dividers);
    int chosen_index = -1;
    for (int i = 0; i < n_avail; i++) {
        if (available_dividers[i] >= fraction) {
            chosen_index = i; break;
        }
    }
    module->regs.clkd.bclk_div = chosen_index;
}

static void config_for_stream(i2s_frame_type_t ftype, int bits_per_sample) {
    module->regs.fmt.lr_period = bits_per_sample - 1;   // bclks per sample
    module->regs.fmt.sample_res = bits_per_sample/4 - 1; // sample resolution = 16
    module->regs.fmt.slot_width = bits_per_sample/4 - 1; // slot width = 16
    module->regs.txchsel[0].offset = 1; // mode is i2s standard: data offset by 1 BCLK relative to LRCK
    module->regs.fifoctl.tx_mode = 1; // if sample width < 32-bit, harvest from LSB of fifo

    int n_output_channels = 2; // output 2-channel (either stereo or duplicate mono)
    uint32_t enable_bits = (1 << n_output_channels) - 1;
    module->regs.txchsel[0].chen = enable_bits;
    module->regs.txchsel[0].chsel = n_output_channels - 1; // set size of map

    if (ftype == I2S_MONO) {
        int n_input_samples_from_fifo = 1; // mono frame consumes 1 sample from fifo
        module->regs.chcfg.tx_slot_num = n_input_samples_from_fifo - 1;
        module->regs.txchmap[0].ch0 = 0; // fill L => first sample
        module->regs.txchmap[0].ch1 = 0; // fill R => first sample (replicate)
    } else {
        int n_input_samples_from_fifo = 2; // stereo frame consumes 2 samples from fifo
        module->regs.chcfg.tx_slot_num = n_input_samples_from_fifo - 1;
        module->regs.txchmap[0].ch0 = 0; // fill L => first sample
        module->regs.txchmap[0].ch1 = 1; // fill R => second sample
    }
}

static void start(void) {
    module->regs.fifoctl.tx_flush = 0;
    module->regs.tx_cntr = 0;
    module->regs.ctl.out_mute = 0; // unmute
    module->regs.ctl.tx_ena = 1;
    module->regs.ctl.global_ena = 1;
}

static void stop(void) {
    module->regs.ctl.out_mute = 1;
    module->regs.ctl.tx_ena = 0;
    module->regs.ctl.global_ena = 0;
}

static void write_fifo(int16_t val) {
    while (module->regs.fsta.tx_ecnt == 0) ; // wait for empty space in fifo
    module->regs.tx_fifo = val;
}

void i2s_play_stream(int sample_freq, const int16_t data[], int ndata, i2s_frame_type_t ftype) {
    int bits_per_sample = 16;
    config_for_stream(ftype, bits_per_sample);
    start();
    // add all samples to stream
    for (int i = 0; i < ndata; /* advance in body */ ) {
        if (ftype == I2S_MONO) {
            write_fifo(data[i++]);
        } else { // stereo
            write_fifo(data[i++]);
            write_fifo(data[i++]);
        }
    }
    stop();
}
