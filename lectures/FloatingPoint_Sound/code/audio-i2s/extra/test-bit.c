// file: test-bit.c
#include<stdio.h>
#include<stdlib.h>
struct I2S_CTL {
    unsigned int GEN: 1;
    unsigned int RXEN: 1;
    unsigned int TXEN: 1;
    unsigned int LOOP: 1;
    unsigned int MODE_SEL: 2;
    unsigned int OUT_MUTE: 1;
    unsigned int UNDEF4: 1;
    unsigned int DOUT0_EN: 1;
    unsigned int DOUT1_EN: 1;
    unsigned int DOUT2_EN: 1;
    unsigned int DOUT3_EN: 1;
    unsigned int UNDEF3: 5;
    unsigned int LRCK_OUT : 1;
    unsigned int BCLK_OUT : 1;
    unsigned int UNDEF2 : 1;
    unsigned int RX_SYNC_EN : 1;
    unsigned int RX_SYNC_EN_START : 1;
    unsigned int UNDEF1 : 10;
};

struct I2S_FMT {
    unsigned int SW : 3;
    unsigned int EDGE_TRANSFER : 1;
    unsigned int SR : 3;
    unsigned int BCLK_POLARITY : 1;
    unsigned int LRCK_PERIOD : 10;
    unsigned int UNUSED1 : 1;
    unsigned int LRCK_POLARITY : 1;
    unsigned int UNUSED2 : 10;
    unsigned int LRCK_WIDTH : 1;
    unsigned int UNUSED3 : 1;
};

struct I2S_ISTA {
    unsigned int RXA_INT : 1;
    unsigned int RXO_INT : 1;
    unsigned int RXU_INT : 1;
    unsigned int UNUSED1 : 1;
    unsigned int TXE_INT : 1;
    unsigned int TXO_INT : 1;
    unsigned int TXU_INT : 1;
    unsigned int UNUSED2 : 25; 
};

struct I2S_FCTL {
    unsigned int RXOM : 2;
    unsigned int TXIM : 1;
    unsigned int UNUSED1 : 1;
    unsigned int RXTL : 6;
    unsigned int UNUSED2 : 2;
    unsigned int TXTL : 7;
    unsigned int UNUSED3 : 5;
    unsigned int FRX : 1;
    unsigned int FTX : 1;
    unsigned int UNUSED4 : 5;
    unsigned int HUB_EN : 1;
};

struct I2S_FSTA {
    unsigned int RXA_CNT : 7;
    unsigned int PLACE_HOLDER : 1;
    unsigned int RXA : 1;
    unsigned int UNUSED1 : 7;
    unsigned int TXE_CNT : 8;
    unsigned int UNUSED2 : 4;
    unsigned int TXE : 1;
    unsigned int UNUSED3 : 3;
};

struct I2S_INT {
    unsigned int RXAI_EN : 1;
    unsigned int RXOI_EN : 1;
    unsigned int RXUI_EN : 1;
    unsigned int RX_DRQ : 1;
    unsigned int TXEI_EN : 1;
    unsigned int TXOI_EN : 1;
    unsigned int TXUI_EN : 1;
    unsigned int TX_DRQ : 1;
    unsigned int UNUSED1 : 24;
};

struct I2S_CLKD {
    unsigned int MCLKDIV : 4;
    unsigned int BCLKDIV : 4;
    unsigned int MCLKO_EN : 1;
    unsigned int UNUSED1 : 23;
};

struct I2S_CHCFG {
    unsigned int TX_SLOT_NUM : 4;
    unsigned int RX_SLOT_NUM : 4;
    unsigned int TX_STATE : 1;
    unsigned int TX_SLOT_HIZ : 1;
    unsigned int UNUSED1 : 22;
};

struct I2S_TXxCHSEL {
    unsigned int TXx_CHEN : 16;
    unsigned int TXx_CHSEL : 4;
    unsigned int TXx_OFFSET : 2;
    unsigned int UNUSED1 : 10;
};

struct I2S_TXxCHMAP0 {
    unsigned int TXx_CH8_MAP : 4;
    unsigned int TXx_CH9_MAP : 4;
    unsigned int TXx_CH10_MAP : 4;
    unsigned int TXx_CH11_MAP : 4;
    unsigned int TXx_CH12_MAP : 4;
    unsigned int TXx_CH13_MAP : 4;
    unsigned int TXx_CH14_MAP : 4;
    unsigned int TXx_CH15_MAP : 4;
};

struct I2S_TXxCHMAP1 {
    unsigned int TXx_CH0_MAP : 4;
    unsigned int TXx_CH1_MAP : 4;
    unsigned int TXx_CH2_MAP : 4;
    unsigned int TXx_CH3_MAP : 4;
    unsigned int TXx_CH4_MAP : 4;
    unsigned int TXx_CH5_MAP : 4;
    unsigned int TXx_CH6_MAP : 4;
    unsigned int TXx_CH7_MAP : 4;
};

struct I2S_RXCHSEL {
    unsigned int UNUSED1 : 16;
    unsigned int RX_CHSEL : 4;
    unsigned int RX_OFFSET : 2;
    unsigned int UNUSED2 : 10;
};

struct I2S_RXCHMAP0 {
    unsigned int RX_CH12_MAP : 4;
    unsigned int RX_CH12_SELECT : 2;
    unsigned int UNUSED1 : 2;
    unsigned int RX_CH13_MAP : 4;
    unsigned int RX_CH13_SELECT : 2;
    unsigned int UNUSED2 : 2;
    unsigned int RX_CH14_MAP : 4;
    unsigned int RX_CH14_SELECT : 2;
    unsigned int UNUSED3 : 2;
    unsigned int RX_CH15_MAP : 4;
    unsigned int RX_CH15_SELECT : 2;
    unsigned int UNUSED4 : 2;
};

struct I2S_RXCHMAP1 {
    unsigned int RX_CH8_MAP : 4;
    unsigned int RX_CH8_SELECT : 2;
    unsigned int UNUSED1 : 2;
    unsigned int RX_CH9_MAP : 4;
    unsigned int RX_CH9_SELECT : 2;
    unsigned int UNUSED2 : 2;
    unsigned int RX_CH10_MAP : 4;
    unsigned int RX_CH10_SELECT : 2;
    unsigned int UNUSED3 : 2;
    unsigned int RX_CH11_MAP : 4;
    unsigned int RX_CH11_SELECT : 2;
    unsigned int UNUSED4 : 2;
};

struct I2S_RXCHMAP2 {
    unsigned int RX_CH4_MAP : 4;
    unsigned int RX_CH4_SELECT : 2;
    unsigned int UNUSED1 : 2;
    unsigned int RX_CH5_MAP : 4;
    unsigned int RX_CH5_SELECT : 2;
    unsigned int UNUSED2 : 2;
    unsigned int RX_CH6_MAP : 4;
    unsigned int RX_CH6_SELECT : 2;
    unsigned int UNUSED3 : 2;
    unsigned int RX_CH7_MAP : 4;
    unsigned int RX_CH7_SELECT : 2;
    unsigned int UNUSED4 : 2;
};

struct I2S_RXCHMAP3 {
    unsigned int RX_CH0_MAP : 4;
    unsigned int RX_CH0_SELECT : 2;
    unsigned int UNUSED1 : 2;
    unsigned int RX_CH1_MAP : 4;
    unsigned int RX_CH1_SELECT : 2;
    unsigned int UNUSED2 : 2;
    unsigned int RX_CH2_MAP : 4;
    unsigned int RX_CH2_SELECT : 2;
    unsigned int UNUSED3 : 2;
    unsigned int RX_CH3_MAP : 4;
    unsigned int RX_CH3_SELECT : 2;
    unsigned int UNUSED4 : 2;
};

struct I2S_MCLKCFG {
    unsigned int ASRC_MCLK_FREQ_DIV_COE : 4;
    unsigned int UNUSED1 : 12;
    unsigned int ASRC_MCLK_GATE : 1;
    unsigned int UNUSED2: 15;
};

struct I2S_FSOUTCFG {
    unsigned int FSOUT_CLK_FREQ_DIV_COE2 : 4;
    unsigned int FSOUT_CLK_FREQ_DIV_COE1 : 4;
    unsigned int UNUSED1 : 8;
    unsigned int FSOUT_CLK_SRC_SEL : 4;
    unsigned int FSOUT_GATE : 1;
    unsigned int UNUSED2 : 11;
};

struct I2S_FsinEXTCFG {
    unsigned int CYCLE_NUM : 16;
    unsigned int EXTEND_ENABLE : 1;
    unsigned int UNUSED1 : 15;
};

struct I2S_ASRCMANCFG {
    unsigned int ASRC_RATIO_VALUE_MANUAL_CFG : 26;
    unsigned int UNUSED1 : 5;
    unsigned int ASRC_RATIO_MANUAL_EN : 1;
};

struct I2S_ASRCRATIOSTAT {
    unsigned int ADAPT_COMPUT_VALUE : 26;
    unsigned int UNUSED1 : 2;
    unsigned int ADAPT_COMPUT_LOCK : 1;
    unsigned int ASRC_BUF_OVERFLOW_STA : 1;
    unsigned int UNUSED2 : 2;
};

struct I2S_ASRCFIFOSTAT {
    unsigned int ASRC_RX_FIFO_FULL_LEVAL : 9;
    unsigned int UNUSED1 : 23;
};

struct I2S_ASRCMBISTCFG {
    unsigned int ASRC_ROM_BIST_EN : 1;
    unsigned int UNUSED1 : 7;
    unsigned int ASRC_RAM_BIST_EN : 1;
    unsigned int UNUSED2 : 23;
};

struct I2S {
    struct I2S_CTL ctl; // offset 0x0
    struct I2S_FMT fmt[2]; // offset 0x4
    struct I2S_ISTA ista; // offset 0xc
    unsigned int rxfifo; // offset 0x10
    struct I2S_FCTL fctl; // offset 0x14
    struct I2S_FSTA fsta; // offset 0x18
    struct I2S_INT i2s_int; // offset 0x1c
    unsigned int txfifo; // offset 0x20
    struct I2S_CLKD clkd; // offset 0x24
    unsigned int txcnt; // offset 0x28
    unsigned int rxcnt; // offset 0x2c
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
    unsigned int UNUSED1; // offset: 0x78
    unsigned int UNUSED2; // offset: 0x7c
    struct I2S_MCLKCFG mclkcfg; // offset 0x80
    struct I2S_FSOUTCFG fsoutcfg; // offset 0x84
    struct I2S_FsinEXTCFG fsinextcfg; // offset 0x88
    unsigned int asrccfg; // not sure of the bits for this one, offset 0x8c
    struct I2S_ASRCMANCFG asrcmancfg; // offset 0x90
    struct I2S_ASRCRATIOSTAT asrcratiostat; // offset 0x94
    struct I2S_ASRCFIFOSTAT asrcfifostat;  // offset 0x98
    struct I2S_ASRCMBISTCFG asrcmbistcfg; // offset 0x9c
    unsigned int asrcmbiststat; // not sure of the bits, offset 0xa0 for this one
};

int main(int argc, char **argv)
{
    printf("sizeof(struct CTRL): %lu\n", sizeof(struct I2S));    
    struct I2S i2s = {0};
    i2s.ctrl.UNDEF1 = 0x3fe;
    i2s.ctrl.GEN = 1;
    printf("%x\n", i2s);
    return 0;
}
