#ifndef I2S_H
#define I2S_H

#include<stdint.h>

/*
 * Hardware abstractions for pulse width modulation (I2S) of
 * digital signals.
 *
 * Author: Chris Gregg <cgregg@stanford.edu>, based on PWM work by 
 * Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: May 27, 2024
 */

typedef enum CHANNEL_TYPE {
    MONO, STEREO
} CHANNEL_TYPE;

// allwinner d1
#define CCU_BASE 0x02001000UL
// unsigned long I2S_0_BASE = 0x02032000;
// unsigned long I2S_1_BASE = 0x02033000;
#define I2S_2_BASE 0x02034000UL
// only I2S2 is exposed on the mango pi

struct I2S_CTL {
    uint32_t GEN: 1;
    uint32_t RXEN: 1;
    uint32_t TXEN: 1;
    uint32_t LOOP: 1;
    uint32_t MODE_SEL: 2;
    uint32_t OUT_MUTE: 1;
    uint32_t UNDEF4: 1;
    uint32_t DOUT0_EN: 1;
    uint32_t DOUT1_EN: 1;
    uint32_t DOUT2_EN: 1;
    uint32_t DOUT3_EN: 1;
    uint32_t UNDEF3: 5;
    uint32_t LRCK_OUT : 1;
    uint32_t BCLK_OUT : 1;
    uint32_t UNDEF2 : 1;
    uint32_t RX_SYNC_EN : 1;
    uint32_t RX_SYNC_EN_START : 1;
    uint32_t UNDEF1 : 10;
};

struct I2S_FMT0 {
    uint32_t SW : 3;
    uint32_t EDGE_TRANSFER : 1;
    uint32_t SR : 3;
    uint32_t BCLK_POLARITY : 1;
    uint32_t LRCK_PERIOD : 10;
    uint32_t UNUSED1 : 1;
    uint32_t LRCK_POLARITY : 1;
    uint32_t UNUSED2 : 10;
    uint32_t LRCK_WIDTH : 1;
    uint32_t UNUSED3 : 1;
};

struct I2S_FMT1 {
    uint32_t TX_PDM : 2;
    uint32_t RX_PDM : 2;
    uint32_t SEXT : 2;
    uint32_t TX_MLS : 1;
    uint32_t RX_MLS : 1;
    uint32_t UNUSED : 24;
};

struct I2S_ISTA {
    uint32_t RXA_INT : 1;
    uint32_t RXO_INT : 1;
    uint32_t RXU_INT : 1;
    uint32_t UNUSED1 : 1;
    uint32_t TXE_INT : 1;
    uint32_t TXO_INT : 1;
    uint32_t TXU_INT : 1;
    uint32_t UNUSED2 : 25; 
};

struct I2S_FCTL {
    uint32_t RXOM : 2;
    uint32_t TXIM : 1;
    uint32_t UNUSED1 : 1;
    uint32_t RXTL : 6;
    uint32_t UNUSED2 : 2;
    uint32_t TXTL : 7;
    uint32_t UNUSED3 : 5;
    uint32_t FRX : 1;
    uint32_t FTX : 1;
    uint32_t UNUSED4 : 5;
    uint32_t HUB_EN : 1;
};

struct I2S_FSTA {
    uint32_t RXA_CNT : 7;
    uint32_t PLACE_HOLDER : 1;
    uint32_t RXA : 1;
    uint32_t UNUSED1 : 7;
    uint32_t TXE_CNT : 8;
    uint32_t UNUSED2 : 4;
    uint32_t TXE : 1;
    uint32_t UNUSED3 : 3;
};

typedef union {
    struct I2S_INT {
        uint32_t RXAI_EN : 1;
        uint32_t RXOI_EN : 1;
        uint32_t RXUI_EN : 1;
        uint32_t RX_DRQ : 1;
        uint32_t TXEI_EN : 1;
        uint32_t TXOI_EN : 1;
        uint32_t TXUI_EN : 1;
        uint32_t TX_DRQ : 1;
        uint32_t UNUSED1 : 24;
    } interrupt;
    uint32_t full;
} I2S_INT;

struct I2S_CLKD {
    uint32_t MCLKDIV : 4;
    uint32_t BCLKDIV : 4;
    uint32_t MCLKO_EN : 1;
    uint32_t UNUSED1 : 23;
};

struct I2S_CHCFG {
    uint32_t TX_SLOT_NUM : 4;
    uint32_t RX_SLOT_NUM : 4;
    uint32_t TX_STATE : 1;
    uint32_t TX_SLOT_HIZ : 1;
    uint32_t UNUSED1 : 22;
};

struct I2S_TXxCHSEL {
    uint32_t TXx_CHEN : 16;
    uint32_t TXx_CHSEL : 4;
    uint32_t TXx_OFFSET : 2;
    uint32_t UNUSED1 : 10;
};

struct I2S_TXxCHMAP0 {
    uint32_t TXx_CH8_MAP : 4;
    uint32_t TXx_CH9_MAP : 4;
    uint32_t TXx_CH10_MAP : 4;
    uint32_t TXx_CH11_MAP : 4;
    uint32_t TXx_CH12_MAP : 4;
    uint32_t TXx_CH13_MAP : 4;
    uint32_t TXx_CH14_MAP : 4;
    uint32_t TXx_CH15_MAP : 4;
};

struct I2S_TXxCHMAP1 {
    uint32_t TXx_CH0_MAP : 4;
    uint32_t TXx_CH1_MAP : 4;
    uint32_t TXx_CH2_MAP : 4;
    uint32_t TXx_CH3_MAP : 4;
    uint32_t TXx_CH4_MAP : 4;
    uint32_t TXx_CH5_MAP : 4;
    uint32_t TXx_CH6_MAP : 4;
    uint32_t TXx_CH7_MAP : 4;
};

struct I2S_RXCHSEL {
    uint32_t UNUSED1 : 16;
    uint32_t RX_CHSEL : 4;
    uint32_t RX_OFFSET : 2;
    uint32_t UNUSED2 : 10;
};

struct I2S_RXCHMAP0 {
    uint32_t RX_CH12_MAP : 4;
    uint32_t RX_CH12_SELECT : 2;
    uint32_t UNUSED1 : 2;
    uint32_t RX_CH13_MAP : 4;
    uint32_t RX_CH13_SELECT : 2;
    uint32_t UNUSED2 : 2;
    uint32_t RX_CH14_MAP : 4;
    uint32_t RX_CH14_SELECT : 2;
    uint32_t UNUSED3 : 2;
    uint32_t RX_CH15_MAP : 4;
    uint32_t RX_CH15_SELECT : 2;
    uint32_t UNUSED4 : 2;
};

struct I2S_RXCHMAP1 {
    uint32_t RX_CH8_MAP : 4;
    uint32_t RX_CH8_SELECT : 2;
    uint32_t UNUSED1 : 2;
    uint32_t RX_CH9_MAP : 4;
    uint32_t RX_CH9_SELECT : 2;
    uint32_t UNUSED2 : 2;
    uint32_t RX_CH10_MAP : 4;
    uint32_t RX_CH10_SELECT : 2;
    uint32_t UNUSED3 : 2;
    uint32_t RX_CH11_MAP : 4;
    uint32_t RX_CH11_SELECT : 2;
    uint32_t UNUSED4 : 2;
};

struct I2S_RXCHMAP2 {
    uint32_t RX_CH4_MAP : 4;
    uint32_t RX_CH4_SELECT : 2;
    uint32_t UNUSED1 : 2;
    uint32_t RX_CH5_MAP : 4;
    uint32_t RX_CH5_SELECT : 2;
    uint32_t UNUSED2 : 2;
    uint32_t RX_CH6_MAP : 4;
    uint32_t RX_CH6_SELECT : 2;
    uint32_t UNUSED3 : 2;
    uint32_t RX_CH7_MAP : 4;
    uint32_t RX_CH7_SELECT : 2;
    uint32_t UNUSED4 : 2;
};

struct I2S_RXCHMAP3 {
    uint32_t RX_CH0_MAP : 4;
    uint32_t RX_CH0_SELECT : 2;
    uint32_t UNUSED1 : 2;
    uint32_t RX_CH1_MAP : 4;
    uint32_t RX_CH1_SELECT : 2;
    uint32_t UNUSED2 : 2;
    uint32_t RX_CH2_MAP : 4;
    uint32_t RX_CH2_SELECT : 2;
    uint32_t UNUSED3 : 2;
    uint32_t RX_CH3_MAP : 4;
    uint32_t RX_CH3_SELECT : 2;
    uint32_t UNUSED4 : 2;
};

struct I2S_MCLKCFG {
    uint32_t ASRC_MCLK_FREQ_DIV_COE : 4;
    uint32_t UNUSED1 : 12;
    uint32_t ASRC_MCLK_GATE : 1;
    uint32_t UNUSED2: 15;
};

struct I2S_FSOUTCFG {
    uint32_t FSOUT_CLK_FREQ_DIV_COE2 : 4;
    uint32_t FSOUT_CLK_FREQ_DIV_COE1 : 4;
    uint32_t UNUSED1 : 8;
    uint32_t FSOUT_CLK_SRC_SEL : 4;
    uint32_t FSOUT_GATE : 1;
    uint32_t UNUSED2 : 11;
};

struct I2S_FsinEXTCFG {
    uint32_t CYCLE_NUM : 16;
    uint32_t EXTEND_ENABLE : 1;
    uint32_t UNUSED1 : 15;
};

struct I2S_ASRCMANCFG {
    uint32_t ASRC_RATIO_VALUE_MANUAL_CFG : 26;
    uint32_t UNUSED1 : 5;
    uint32_t ASRC_RATIO_MANUAL_EN : 1;
};

struct I2S_ASRCRATIOSTAT {
    uint32_t ADAPT_COMPUT_VALUE : 26;
    uint32_t UNUSED1 : 2;
    uint32_t ADAPT_COMPUT_LOCK : 1;
    uint32_t ASRC_BUF_OVERFLOW_STA : 1;
    uint32_t UNUSED2 : 2;
};

struct I2S_ASRCFIFOSTAT {
    uint32_t ASRC_RX_FIFO_FULL_LEVAL : 9;
    uint32_t UNUSED1 : 23;
};

struct I2S_ASRCMBISTCFG {
    uint32_t ASRC_ROM_BIST_EN : 1;
    uint32_t UNUSED1 : 7;
    uint32_t ASRC_RAM_BIST_EN : 1;
    uint32_t UNUSED2 : 23;
};

struct I2S_ASRCMBISTSTAT {
    uint32_t RAM_BUSY_STATUS : 1;
    uint32_t RAM_STOP_STATUS : 1;
    uint32_t RAM_BIST_ERROR_CYCLE : 2;
    uint32_t RAM_BIST_ERROR_PATTERN : 3;
    uint32_t RAM_BIST_ERR_STATUS : 1;
    uint32_t UNUSED1 : 8;
    uint32_t ROM_BUSY_STATUS : 1;
    uint32_t ROM_BIST_ERROR_SUM : 1;
    uint32_t ROM_BIST_ERROR_XOR : 1;
    uint32_t UNUSED2 : 13;
};

typedef union {
    struct I2S {
        struct I2S_CTL ctl; // offset 0x0
        struct I2S_FMT0 fmt0; // offset 0x4
        struct I2S_FMT1 fmt1; // offset 0x8
        struct I2S_ISTA ista; // offset 0xc
        uint32_t rxfifo; // offset 0x10
        struct I2S_FCTL fctl; // offset 0x14
        struct I2S_FSTA fsta; // offset 0x18
        I2S_INT i2s_int; // offset 0x1c
        uint32_t txfifo; // offset 0x20
        struct I2S_CLKD clkd; // offset 0x24
        uint32_t txcnt; // offset 0x28
        uint32_t rxcnt; // offset 0x2c
        struct I2S_CHCFG chcfg; // offset 0x30
        struct I2S_TXxCHSEL txxchsel[4]; // offset 0x34
        struct I2S_TXxCHMAP0 tx0chmap0; // offset 0x44
        struct I2S_TXxCHMAP1 tx0chmap1; // offset 0x48
        struct I2S_TXxCHMAP0 tx1chmap0; // offset 0x4c
        struct I2S_TXxCHMAP1 tx1chmap1; // offset 0x50
        struct I2S_TXxCHMAP0 tx2chmap0; // offset 0x54
        struct I2S_TXxCHMAP1 tx2chmap1; // offset 0x58
        struct I2S_TXxCHMAP0 tx3chmap0; // offset 0x5c
        struct I2S_TXxCHMAP1 tx3chmap1; // offset 0x60
        struct I2S_RXCHSEL rxchsel; // offset 0x64
        struct I2S_RXCHMAP0 rxchmap0; // offset 0x68
        struct I2S_RXCHMAP1 rxchmap1; // offset 0x6c
        struct I2S_RXCHMAP2 rxchmap2; // offset 0x70
        struct I2S_RXCHMAP3 rxchmap3; // offset 0x74
        uint32_t UNUSED1; // offset: 0x78
        uint32_t UNUSED2; // offset: 0x7c
        struct I2S_MCLKCFG mclkcfg; // offset 0x80
        struct I2S_FSOUTCFG fsoutcfg; // offset 0x84
        struct I2S_FsinEXTCFG fsinextcfg; // offset 0x88
        uint32_t asrccfg; // offset 0x8c
        struct I2S_ASRCMANCFG asrcmancfg; // offset 0x90
        struct I2S_ASRCRATIOSTAT asrcratiostat; // offset 0x94
        struct I2S_ASRCFIFOSTAT asrcfifostat;  // offset 0x98
        struct I2S_ASRCMBISTCFG asrcmbistcfg; // offset 0x9c
        uint32_t I2S_ASRCMBISTSTAT; // offset 0xa0
    } regs;
    uint32_t all[168];
} I2S;

struct PLL_AUDIO0_CTRL {
    uint32_t PLL_OUTPUT_DIV2   : 1;
    uint32_t PLL_INPUT_DIV2    : 1;
    uint32_t UNUSED1           : 3;
    uint32_t PLL_LOCK_MDSEL    : 1;
    uint32_t PLL_UNLOCK_MDSEL  : 2;
    uint32_t PLL_N             : 8;
    uint32_t PLL_P             : 6;
    uint32_t UNUSED2           : 2;
    uint32_t PLL_SDM_EN        : 1;
    uint32_t UNUSED3           : 2;
    uint32_t PLL_OUTPUT_GATE   : 1;
    uint32_t LOCK              : 1;
    uint32_t LOCK_ENABLE       : 1;
    uint32_t PLL_LDO_EN        : 1;
    uint32_t PLL_EN            : 1;
};

struct I2S_CLK {
    uint32_t FACTOR_M : 5;
    uint32_t UNUSED1  : 3;
    uint32_t FACTOR_N : 2;
    uint32_t UNUSED2  : 14;
    uint32_t CLK_SRC_SEL  : 3;
    uint32_t UNUSED3  : 4;
    uint32_t I2S_CLK_GATING : 1;
};

struct I2S_ASRC_CLK {
    uint32_t FACTOR_M : 5;
    uint32_t UNUSED1  : 3;
    uint32_t FACTOR_N : 2;
    uint32_t UNUSED2  : 14;
    uint32_t CLK_SRC_SEL  : 3;
    uint32_t UNUSED3  : 4;
    uint32_t I2S_CLK_GATING : 1;
};



struct I2S_PCM_BGR {
    uint32_t I2S0_GATING : 1;
    uint32_t I2S1_GATING : 1;
    uint32_t I2S2_GATING : 1;
    uint32_t UNUSED1 : 13;
    uint32_t I2S0_RST : 1;
    uint32_t I2S1_RST : 1;
    uint32_t I2S2_RST : 1;
    uint32_t UNUSED2 : 13;
};
/*
 * Initialize the i2s module. 
 */
void i2s_init(void);

/*
 * Set the I2S clock frequency.
 *
 * @param frequency      the frequency of the I2S clock
*/
void i2s_set_clock(int frequency);

/*
 * Setup I2S
 */
void i2s_setup(int frequency, int block_alignment);
void i2s_mic_setup(int frequency);

/*
 * Enable or disable I2S.
 *
 * @param chan      the I2S channel - either 0 or 1
 */
void i2s_enable(CHANNEL_TYPE channel_type);
void i2s_enable_interrupts();
void i2s_start();
void i2s_disable(int chan);
void i2s_mic_enable();
void i2s_enable_mic_interrupts();

/*
 * Write a value to both channels
 *
 */
void i2s_write_stereo(uint16_t left, uint16_t right);

/*
 * Write a value to one channel
 *
 */
void i2s_write_mono(uint16_t value);


/*
 * Read the current status of the I2S channel.
 *
 * @return the current status of the channels. If I2S_FULL1 is set, then
 * need to wait until the channel is not full before writing a new value.
 */
unsigned int i2s_get_status(void);

void i2s_mic_start();

#endif
