#include "timer.h"
#include "gpio.h"
#include "i2s.h"
#include "stdint.h"
#include "ccu.h"
#include "printf.h"
#include "malloc.h"
#include "i2s.h"

// this code works for the HiLetgo PCM5102 I2S IIS Lossless Digital Audio DAC Decoder Module Stereo DAC Digital-to-Analog Converter Voice Module 
// Connect VIN on device to 3.3V, and GND on device to ground
// The only other necessary pins are BCK, DIN, and LCK. All other pins can be left floating. 
// Datasheet: https://www.ti.com/lit/ds/symlink/pcm5102a.pdf?ts=1716798075863&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FPCM5102A

// these pins are for I2S2:
#define DOUT0 GPIO_PB4 // goes to DIN on device
#define DOUT1 GPIO_PB3 // not needed
#define DOUT2 GPIO_PB2 // not needed
#define DOUT3 GPIO_PB1 // not needed
#define BCLK  GPIO_PB5 // goes to BCK on device
#define LRCK  GPIO_PB6 // goes to LCK on device
#define MCLK  GPIO_PB7 // not needed
#define DIN0  GPIO_PB3 // mic input data
 
                       
                       
                       
#define PLL_AUDIO0_CTRL_REG CCU_BASE + 0x78
#define PLL_AUDIO1_CTRL_REG CCU_BASE + 0x80
#define I2S2_CLK_REG (CCU_BASE + 0xA18)
#define I2S2_ASRC_CLK_REG (CCU_BASE + 0xA1C)
#define I2S_PCM_BGR_REG (CCU_BASE + 0xA20)

volatile I2S *i2s2;

void i2s_init(void)
{
}

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


void i2s_enable(CHANNEL_TYPE channel_type) 
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
    // i2s2->regs.clkd.MCLKO_EN = 1;
    i2s2->regs.ctl.DOUT0_EN = 1;
    // i2s2->regs.ctl.DOUT1_EN = 0;
    // i2s2->regs.ctl.DOUT2_EN = 0;
    // i2s2->regs.ctl.DOUT3_EN = 0;
    // printf("mclko after: %x\n", *(uint32_t *)&(i2s2->regs.clkd));
   
    // working (non-stereo):
    if (channel_type == MONO) {
        i2s2->regs.fmt0.LRCK_PERIOD = 15; 
        i2s2->regs.chcfg.TX_SLOT_NUM = 0x0; 
        i2s2->regs.txxchsel[0].TXx_CHEN = 0x3;
        i2s2->regs.txxchsel[0].TXx_CHSEL = 0x1;
        i2s2->regs.txxchsel[0].TXx_OFFSET = 0x1;
    } else {
        // working for stereo:
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
    }
    
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

    
    /*
    i2s2->regs.txxchsel[1].TXx_CHEN = 0xf;
    i2s2->regs.txxchsel[1].TXx_CHSEL = 0xf;
    i2s2->regs.txxchsel[1].TXx_OFFSET = 1;
    */

    i2s2->regs.fctl.FTX = 0;
    i2s2->regs.fctl.FRX = 0;
    i2s2->regs.txcnt = 0; 
    i2s2->regs.rxcnt = 0; 

}

void i2s_enable_interrupts() {
    i2s2->regs.i2s_int.interrupt.TX_DRQ = 1;
    i2s2->regs.i2s_int.interrupt.TXUI_EN = 1;
    i2s2->regs.i2s_int.interrupt.TXOI_EN = 1;
    i2s2->regs.i2s_int.interrupt.TXEI_EN = 1;
}

void i2s_start() {
    i2s2->regs.ctl.TXEN = 1;
    //////i2s2->regs.ctl.RXEN = 1;
    i2s2->regs.ctl.GEN = 1;
    i2s2->regs.txcnt = 0;
}

unsigned i2s_get_status(void) {
    // printf("TXFIFO Empty Space Word Counter: %x\n", *(uint32_t *)&i2s2->regs.fsta);
    uint32_t status = i2s2->regs.fsta.TXE;
    return status;
}

void i2s_write_stereo(uint16_t left, uint16_t right) {
    i2s2->regs.txfifo = left << 16;
    i2s2->regs.txfifo = right << 16;
}

void i2s_write_mono(uint16_t value) {
    i2s2->regs.txfifo = value << 16;
}

void i2s_mic_setup(int frequency) {
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
    int block_div_frac = frequency / 44100 * 8 / 4; // 4 was block_alignment
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


void i2s_mic_enable() 
{
    gpio_set_function( DIN0, GPIO_FN_ALT5 );  
    gpio_set_function( BCLK,  GPIO_FN_ALT3 );  
    gpio_set_function( LRCK,  GPIO_FN_ALT3 );  
    // gpio_set_function( MCLK,  GPIO_FN_ALT3 );  

    // printf("mclko before: %x\n", *(uint32_t *)&(i2s2->regs.clkd));
    i2s2->regs.clkd.MCLKDIV = 0x2;
    // i2s2->regs.clkd.MCLKO_EN = 1;
    // i2s2->regs.ctl.DOUT0_EN = 1;
    // printf("mclko after: %x\n", *(uint32_t *)&(i2s2->regs.clkd));
  
    /* original try: 
    // mono 
    i2s2->regs.fmt0.LRCK_PERIOD = 31; 
    i2s2->regs.chcfg.RX_SLOT_NUM = 0x0; 
    i2s2->regs.rxchsel.RX_CHSEL = 0x1;
    i2s2->regs.rxchsel.RX_OFFSET = 0x1;
    */
    i2s2->regs.fmt0.LRCK_PERIOD = 31; 
    i2s2->regs.chcfg.RX_SLOT_NUM = 0x0; 
    i2s2->regs.rxchsel.RX_CHSEL = 0x0;
    i2s2->regs.rxchsel.RX_OFFSET = 0x1;
    i2s2->regs.rxchmap3.RX_CH0_SELECT = 0;
    i2s2->regs.rxchmap3.RX_CH0_MAP = 0;
    i2s2->regs.fmt0.SR = 0x7; // 32-bit sample resolution
    i2s2->regs.fmt0.SW = 0x7; // 32-bit slot width (?) 
    // i2s2->regs.ctl.RX_SYNC_EN = 1;
   /* try 2: 
    i2s2->regs.fmt0.SW = 0x3; // 16-bit slot width (?) 
    i2s2->regs.fmt0.SR = 0x7; // 16-bit sample resolution
    // i2s2->regs.fmt1.SEXT = 0x3;

    i2s2->regs.fmt0.LRCK_PERIOD = 31; 
    i2s2->regs.chcfg.RX_SLOT_NUM = 0x1; // two slots (L/R)
    // i2s2->regs.rxchsel.RX_CHEN = 0x3; // enable both slots 
    i2s2->regs.rxchsel.RX_CHSEL = 0x1; // 1 Channel (Slot) Number Select for Each Output
    i2s2->regs.rxchmap3.RX_CH0_MAP = 0;
    i2s2->regs.rxchmap3.RX_CH1_MAP = 1; 
    i2s2->regs.rxchmap3.RX_CH0_SELECT = 0; 
    i2s2->regs.rxchmap3.RX_CH1_SELECT = 0; 
    i2s2->regs.rxchsel.RX_OFFSET = 1;
    */

    
    // i2s2->regs.fctl.FTX = 0;
    i2s2->regs.fctl.FRX = 0;
    i2s2->regs.rxcnt = 0; 
}

void i2s_enable_mic_interrupts() {
    i2s2->regs.i2s_int.interrupt.RX_DRQ = 1;
    i2s2->regs.i2s_int.interrupt.RXUI_EN = 1;
    i2s2->regs.i2s_int.interrupt.RXOI_EN = 1;
    i2s2->regs.i2s_int.interrupt.RXAI_EN = 1;
}

void i2s_mic_start() {
    // i2s2->regs.ctl.RX_SYNC_EN_START = 1;
    i2s2->regs.ctl.RXEN = 1;
    //////i2s2->regs.ctl.RXEN = 1;
    i2s2->regs.ctl.GEN = 1;
    i2s2->regs.txcnt = 0;
}
