#include "timer.h"
#include "gpio.h"
#include "i2s.h"
#include "stdint.h"
#include "ccu.h"
#include "printf.h"

// allwinner d1
unsigned long CCU_BASE   = 0x02001000;
// unsigned long I2S_0_BASE = 0x02032000;
// unsigned long I2S_1_BASE = 0x02033000;
unsigned long I2S_2_BASE = 0x02034000;

// these pins are for I2S2:
#define DOUT0 GPIO_PB4
#define DOUT1 GPIO_PB3
#define DOUT2 GPIO_PB2
#define DOUT3 GPIO_PB1
#define BCLK  GPIO_PB5
#define LRCK  GPIO_PB6
#define MCLK  GPIO_PB7
#define PLL_AUDIO0_CTRL_REG CCU_BASE + 0x78
#define PLL_AUDIO1_CTRL_REG CCU_BASE + 0x80
#define I2S2_CLK_REG (CCU_BASE + 0xA18)
#define I2S2_ASRC_CLK_REG (CCU_BASE + 0xA1C)
#define I2S_PCM_BGR_REG (CCU_BASE + 0xA20)

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
};

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
        struct I2S_INT i2s_int; // offset 0x1c
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

volatile I2S *i2s2;

void i2s_init(void)
{
}

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

void i2s_setup(int frequency, int block_alignment) {
    volatile struct PLL_AUDIO0_CTRL *pll_audio0_ctrl = (struct PLL_AUDIO0_CTRL *)(PLL_AUDIO0_CTRL_REG); 
//    volatile struct PLL_AUDIO0_CTRL *pll_audio1_ctrl = (struct PLL_AUDIO0_CTRL *)(PLL_AUDIO1_CTRL_REG); 

    volatile struct I2S_CLK *i2s_clk_reg = (struct I2S_CLK *)(I2S2_CLK_REG); 
    // volatile struct I2S_ASRC_CLK *i2s_asrc_clk_reg = (struct I2S_ASRC_CLK *)(I2S2_ASRC_CLK_REG); 

    i2s2 = (I2S *)I2S_2_BASE;
    printf("original value of pll_audio0_ctrl: %x\n", *(uint32_t *)pll_audio0_ctrl);
//    printf("original value of pll_audio1_ctrl: %x\n", *(uint32_t *)pll_audio1_ctrl);
    /* we'll use the default for now 
    // Firstly, disable the PLL_AUDIO through PLL_AUDIOx Control Register[PLL_ENABLE] in the CCU. (page 654)
    ccu_write(0x78, ccu_read(0x78) & 0x7FFFFFFF);
    */

    // Secondly, set up the frequency of the PLL_AUDIO in the PLL_AUDIOx Control Register.
    // PLL_AUDIO0(1X) = (24MHz*N/M1/M0)/P/4
    // The default frequency of PLL_AUDIO0(1X) is 24.5714 MHz (calc based on numbers below: 24 * (0b01010101 + 1) / 1 / 1 / (0b010100 + 1)  / 4)
    // default for register 0x78 is 0x48145500, or 0b 0100 1000 0001 0100 0101 0101 0000 0000
    // bits:      default:
    // 31: PLL_EN       0
    // 30: PLL_LDO_EN   1
    // 29: LOCK_ENABLE  0
    // 28: LOCK         0
    // 27: PLL_OUTPUT_GATE 1
    // 26:25 unused     00
    // 24: PLL_SDM_EN   0
    // 23:22 unused     00
    // 21:16 PLL_P      01 0100, P= PLL_POST_DIV_P +1
    // 15:8 PLL_N       0101 0101, N= PLL_N +1
    // 7:6 PLL_UNLOCK_MDSEL 00
    // 5: PLL_LOCK_MDSEL 0
    // 4:2 unused        000
    // 1: PLL_INPUT_DIV2 M1 0, M1=PLL_INPUT_DIV2 + 1
    // 0: PLL_OUTPUT_DIV2 M0 0, M0=PLL_OUTPUT_DIV2 + 1

    // After that, enable the I2S/PCM gating through the I2S/PCMx_CLK_REG when you checkout that the PLL_AUDIOx Control Register[LOCK] becomes to 1.
 
    // page 46: Configuring the Frequency of General PLLs (will configure audio0)
    // Make sure the PLL is enabled.
    // to enable:
    // Step 1 Configure the N, M, and P factors of the PLL control register.
    // (using defaults for now)
    // Step 2 Write the PLL_ENABLE bit and the LDO_EN bit of the PLL control register to 1, write the
    // PLL_OUTPUT_GATE bit of the PLL control register to 0.
    pll_audio0_ctrl->PLL_EN = 1;  // enable pll audio
    // pll_audio0_ctrl->PLL_LDO_EN = 0;  // enable pll ldo audio
    pll_audio0_ctrl->PLL_OUTPUT_GATE = 0;  // output gate 

    // Step 3 Write the LOCK_ENABLE bit of the PLL control register to 1.
    pll_audio0_ctrl->LOCK_ENABLE = 1;

    // Step 4 Wait for the status of the Lock to change to 1.
    while (pll_audio0_ctrl->LOCK != 1) {}
    // Step 5 Delay 20 us.
    timer_delay_ms(20);

    // pll_audio0_ctrl->LOCK_ENABLE = 0; 
    pll_audio0_ctrl->PLL_EN = 0;  // disable pll audio
                                  // setup frequency (using default)
    // pll_audio0_ctrl->PLL_N = 254;
    // pll_audio0_ctrl->PLL_P = 63;
    pll_audio0_ctrl->PLL_N = 244;
    pll_audio0_ctrl->PLL_P = 63;
    pll_audio0_ctrl->LOCK_ENABLE = 1; 
    while (pll_audio0_ctrl->LOCK != 1) {}
    // delay 20us
    timer_delay_ms(20);
    pll_audio0_ctrl->PLL_OUTPUT_GATE = 1;
    pll_audio0_ctrl->PLL_EN = 1;

    printf("audio0 locked! new value: %x\n", *(uint32_t *)pll_audio0_ctrl);

    // Enabling the PLL Follow the steps below to enable the PLL:
    // Step 1 Configure the N, M, and P factors of the PLL control register.
    // (using defaults)
    // Step 2 Write the PLL_ENABLE bit and the LDO_EN bit of the PLL control register to 1, write the PLL_OUTPUT_GATE bit of the PLL control register to 0.
    pll_audio0_ctrl->PLL_EN = 1; 
    pll_audio0_ctrl->PLL_LDO_EN = 1; 
    pll_audio0_ctrl->PLL_OUTPUT_GATE = 0;

    // Step 3 Write the LOCK_ENABLE bit of the PLL control register to 1.
    pll_audio0_ctrl->LOCK_ENABLE = 1; 
    // Step 4 Wait for the status of the Lock to change to 1.
    while (pll_audio0_ctrl->LOCK != 1) {}
    // Step 5 Delay 20 us.
    timer_delay_ms(20);
    // Step 6 Write the PLL_OUTPUT_GATE bit of the PLL control register to 1 and then the PLL will be available.
    pll_audio0_ctrl->PLL_OUTPUT_GATE = 1;
    printf("audio0 locked again. new value: %x\n", *(uint32_t *)pll_audio0_ctrl);

     
    i2s_clk_reg->I2S_CLK_GATING = 1;
    i2s_clk_reg->CLK_SRC_SEL = 0; // AUDIO(1X)
    printf("i2s2_clk_reg: %x\n", *(uint32_t *)i2s_clk_reg);

    //  At last, reset and enable the I2S/PCM bus gating by setting I2S/PCM_BGR_REG.
    volatile struct I2S_PCM_BGR *i2s_pcm_bgr = (struct I2S_PCM_BGR *)(I2S_PCM_BGR_REG); 
    printf("pcm_bgr before: %x\n", *(uint32_t *)i2s_pcm_bgr);
    i2s_pcm_bgr->I2S2_RST = 1;
    i2s_pcm_bgr->I2S2_GATING = 1;
    printf("pcm_bgr after: %x\n", *(uint32_t *)i2s_pcm_bgr);
    // MAYBE WE HAVE TO SET BACK TO 0??
    // i2s_pcm_bgr->I2S2_RST = 0;

    // Firstly, initialize the I2S/PCM. You should close the Globe Enable 
    // bit (I2S/PCM_CTL[0]), Transmitter Block Enable bit (I2S/PCM_CTL[2]), 
    // and Receiver Block Enable bit (I2S/PCM_CTL[1]) by writing 0. 
    printf("i2s2->ctl before: %x\n", *(uint32_t *)&(i2s2->regs.ctl));
    // for (uint32_t i = 0; i < 0xa0; i+=4) {
    //     printf("offset: %x, value:%x\n", i, *(uint32_t *)(0x02032000UL + i));
    //     // printf("offset: %x, value:%x\n", i, *(uint32_t *)(0x02001000UL + i));
    // }
    i2s2->regs.ctl.GEN = 0;
    i2s2->regs.ctl.RXEN = 0;
    i2s2->regs.ctl.TXEN = 0;
    printf("is2->ctl after: %x\n", *(uint32_t *)&(i2s2->regs.ctl));
    // After that, clear the TX/RX FIFO by writing 0 to the
    // bit[25:24] of I2S/PCM_FCTL.
    i2s2->regs.fctl.FTX = 0;
    i2s2->regs.fctl.FRX = 0;
    printf("is2->fctl after: %x\n", *(uint32_t *)&(i2s2->regs.fctl));
    // At last, you can clear the TX FIFO and RX FIFO counter by writing 0 to
    // I2S/PCM_TXCNT and I2S/PCM_RXCNT.
    i2s2->regs.txcnt = 0; 
    i2s2->regs.rxcnt = 0; 

    // First, you can set up the I2S/PCM of master and slave. The configuration 
    // can be referred to the protocol of I2S/PCM. 
    i2s2->regs.ctl.BCLK_OUT = 1;
    i2s2->regs.ctl.LRCK_OUT = 1;

    // Then, you can set up the translation mode, the sample resolution, 
    // the wide of the slot, the channel slot number, and the trigger level, and so on. 
    // The setup of the register can be found in the specification.
    i2s2->regs.ctl.MODE_SEL = 1; // left justified (for PCM5102A chip)
    i2s2->regs.ctl.OUT_MUTE = 0; 
    
    printf("is2->fmt0 before: %x\n", *(uint32_t *)&(i2s2->regs.fmt0));
    // i2s2->regs.fmt0.SR = 0x3; // 16-bit sample resolution
    // i2s2->regs.fmt0.SW = 0x3; // 16-bit slot width (?) 
    //i2s2->regs.fmt0.LRCK_PERIOD = 31; 
    int block_div_frac = frequency / 44100 * 8 / block_alignment; 
    switch (block_div_frac) {
       case 1:
           i2s2->regs.clkd.BCLKDIV = 0x1;
           break;
       case 2:
           i2s2->regs.clkd.BCLKDIV = 0x2;
           break;
       case 4:
           i2s2->regs.clkd.BCLKDIV = 0x3;
           break;
       default:
           i2s2->regs.clkd.BCLKDIV = 0x2;
    }
    
    printf("is2->fmt0 after: %x\n", *(uint32_t *)&(i2s2->regs.fmt0));
    printf("finished.\n");
}


void i2s_enable() 
{
    gpio_set_function( DOUT0, GPIO_FN_ALT3 );  
    /*
    gpio_set_function( DOUT1, GPIO_FN_ALT3 );  
    gpio_set_function( DOUT2, GPIO_FN_ALT3 );  
    gpio_set_function( DOUT3, GPIO_FN_ALT3 );  
    */
    gpio_set_function( BCLK,  GPIO_FN_ALT3 );  
    gpio_set_function( LRCK,  GPIO_FN_ALT3 );  
    // gpio_set_function( MCLK,  GPIO_FN_ALT3 );  

    // printf("mclko before: %x\n", *(uint32_t *)&(i2s2->regs.clkd));
    i2s2->regs.clkd.MCLKDIV = 0x1;
    i2s2->regs.clkd.MCLKO_EN = 1;
    i2s2->regs.ctl.DOUT0_EN = 1;
    // i2s2->regs.ctl.DOUT1_EN = 0;
    // i2s2->regs.ctl.DOUT2_EN = 0;
    // i2s2->regs.ctl.DOUT3_EN = 0;
    // printf("mclko after: %x\n", *(uint32_t *)&(i2s2->regs.clkd));
   
    // working: 
    // i2s2->regs.fmt0.LRCK_PERIOD = 15; 
    // i2s2->regs.chcfg.TX_SLOT_NUM = 0x0; 
    // i2s2->regs.txxchsel[0].TXx_CHEN = 0x3;
    // i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1;
    // i2s2->regs.txxchsel[0].TXx_OFFSET = 0x1;
    
    /* left channel only:
    i2s2->regs.fmt0.LRCK_PERIOD = 15; 
    i2s2->regs.chcfg.TX_SLOT_NUM = 0x0; // 1 slot
    i2s2->regs.txxchsel[0].TXx_CHEN = 0x1; // enable slot 1 (left)
    i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1; // 1 slot
    i2s2->regs.txxchsel[0].TXx_OFFSET = 1;
    */
   
    /* right channel only: 
    i2s2->regs.fmt0.LRCK_PERIOD = 15; 
    i2s2->regs.chcfg.TX_SLOT_NUM = 0x0; // 1 slot
    i2s2->regs.txxchsel[0].TXx_CHEN = 0x2; // enable slot 2 (right)
    i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1; // 1 slot
    i2s2->regs.txxchsel[0].TXx_OFFSET = 1;
    */

    /*
    // left only:
    i2s2->regs.fmt0.LRCK_PERIOD = 15; 
    i2s2->regs.chcfg.TX_SLOT_NUM = 0x0; // one slot
    i2s2->regs.txxchsel[0].TXx_CHEN = 0x1; // enable channel 1
    i2s2->regs.txxchsel[0].TXx_CHSEL = 0x0; // one channel total
    i2s2->regs.txxchsel[0].TXx_OFFSET = 1;
    */
  
    /* 
    // right only:
    i2s2->regs.fmt0.LRCK_PERIOD = 15; 
    i2s2->regs.chcfg.TX_SLOT_NUM = 0x0; // one slot
    i2s2->regs.txxchsel[0].TXx_CHEN = 0x2; // enable channel 2
    i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1; // two channels total
    i2s2->regs.txxchsel[0].TXx_OFFSET = 1;
    */
    i2s2->regs.fmt0.SW = 0x3; // 16-bit slot width (?) 
    i2s2->regs.fmt0.SR = 0x7; // 16-bit sample resolution
    // i2s2->regs.fmt1.SEXT = 0x3;

    i2s2->regs.fmt0.LRCK_PERIOD = 15; 
    i2s2->regs.chcfg.TX_SLOT_NUM = 0x1; // two slots (L/R)
    i2s2->regs.txxchsel[0].TXx_CHEN = 0x3; // enable both slots 
    i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1; // 1 Channel (Slot) Number Select for Each Output
    i2s2->regs.tx0chmap1.TXx_CH0_MAP = 0;
    i2s2->regs.tx0chmap1.TXx_CH1_MAP = 1; 
    i2s2->regs.txxchsel[0].TXx_OFFSET = 1;
    
    /*
    i2s2->regs.txxchsel[1].TXx_CHEN = 0xf;
    i2s2->regs.txxchsel[1].TXx_CHSEL = 0xf;
    i2s2->regs.txxchsel[1].TXx_OFFSET = 1;
    */

    i2s2->regs.fctl.FTX = 0;
    i2s2->regs.fctl.FRX = 0;
    i2s2->regs.txcnt = 0; 
    i2s2->regs.rxcnt = 0; 
    i2s2->regs.ctl.TXEN = 1;
    //i2s2->regs.ctl.RXEN = 1;
    i2s2->regs.ctl.GEN = 1;
    i2s2->regs.txcnt = 0;
}

unsigned i2s_get_status(void) {
    // printf("TXFIFO Empty Space Word Counter: %x\n", *(uint32_t *)&i2s2->regs.fsta);
    uint32_t status = i2s2->regs.fsta.TXE;
    return status;
}

void i2s_write(uint16_t left, uint16_t right) {
    i2s2->regs.txfifo = left << 16;
    i2s2->regs.txfifo = right << 16;
    /*
    if (i2s2->regs.txxchsel[0].TXx_CHEN == 0x2) {
        i2s2->regs.txxchsel[0].TXx_CHEN = 0x1;
        i2s2->regs.txxchsel[0].TXx_CHSEL = 0x0;
    } else {
        i2s2->regs.txxchsel[0].TXx_CHEN = 0x2;
        i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1;
    }
    */
}
