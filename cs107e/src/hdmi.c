/*
 * This module configures the HDMI hardware
 * Written to drive Synopsis DesignWare HDMI TX controller using in AW D1.
 * Uses TCONTV peripheral of AW D1 to stream pixels to HDMI
 * 
 * Support for classic resolutions: 1080p, 720p, SVGA
 * 
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Updated: Feb 2024
 */

#include "hdmi.h"
#include "_hdmi_private.h"
#include "assert.h"
#include "ccu.h"
#include "printf.h"
#include <stdbool.h>
#include "timer.h"

typedef union {
    struct {
        uint8_t invidconf;          // V and H sync polarity bits 5&6, data enble input polarity bit 4, HDMI mode bit 3
        uint8_t inhactv[2];         // count of horiz active pixels
        uint8_t inhblank[2];        // count of horiz blank pixels
        uint8_t invactv[2];         // count of vert active lines
        uint8_t invblank;           // count of vert blank lines
        uint8_t hsyncindelay[2];    // count of pixel clock cycles from non-active edge to last valid period
        uint8_t hsyncinwidth[2];    // count of pixel clock cycles
        uint8_t vsyncindelay;       // count of hsync pulses from non-active edge to last valid period
        uint8_t vsyncinwidth;       // count of hsync pulses
        uint8_t infreq[3];          // these fields used for debugging
        uint8_t ctrldur;            // control period minimum duration (min 12 pixel clock cycles)
        uint8_t exctrldur;          // extended control period minimum duration (min 32 pixel clock cycles)
        uint8_t exctrlspac;         // extended control period maximum spacing (max 50 msec)
        uint8_t chpream[3];         // bits to fill channel data lines not used to transmit the preamble
    } regs;
} hdmi_frame_composer_t;

typedef union {
    struct {
        uint8_t sfrdiv;
        uint8_t clkdis;         // clock domain disable
        uint8_t fswrstz;
        uint8_t opctrl;
        uint8_t flowctrl;
     } regs;
} hdmi_main_controller_t;

typedef union {
    struct {
        uint32_t setup;
        uint32_t reserved[6];
        uint32_t port_sel;
        uint32_t gate;
    } regs;
} tcon_top_t;

typedef union {
    struct {
        uint32_t gtcl;
        uint32_t reserved[15];
        uint32_t src_ctl;
        uint32_t reservedB[19];
        uint32_t ctl;
        // next 6 regs are named basic0-basic5 in doc
        struct { uint32_t height:16, width:16; } dimensions[3];
        struct { uint32_t bp:16, total:16; } htiming, vtiming;
        struct { uint32_t vert:16, horiz:16; } sync;
    } regs;
} tcon_tv_t;


struct display_timing {
    hdmi_resolution_id_t id;
    struct {
        uint32_t pixels, front_porch, sync_pulse, back_porch;
    } horiz, vert;
    uint32_t pixel_clock_hz;
    uint32_t pll_m, pll_n;      // dividers for PLLVideo0
    uint32_t tcon_n, tcon_m;    // dividers for TCONTV
    uint32_t de_m;              // divider for DE clock
};

#define HDMI_FC   ((hdmi_frame_composer_t *)0x5501000)
#define HDMI_MC  ((hdmi_main_controller_t *)0x5504000)
#define TCON_TOP             ((tcon_top_t *)0x5460000)
#define TCON_TV               ((tcon_tv_t *)0x5470000)

_Static_assert(&(HDMI_FC->regs.invblank)  ==  (uint8_t *)0x5501007, "hdmi fc invblank reg must be at address 0x5501007");
_Static_assert(&(HDMI_MC->regs.clkdis)    ==  (uint8_t *)0x5504001, "hdmi mc clkdis reg must be at address 0x5504001");
_Static_assert(&(TCON_TOP->regs.port_sel) == (uint32_t *)0x546001c, "tcon top port_sel reg must be at address 0x546001c");
_Static_assert(&(TCON_TV->regs.src_ctl)   == (uint32_t *)0x5470040, "tcon tv src_ctl eg must be at address 0x5470040");

static struct {
    volatile hdmi_frame_composer_t *hdmi_fc;
    volatile hdmi_main_controller_t *hdmi_mc;
    volatile tcon_top_t *tcon_top;
    volatile tcon_tv_t *tcon_tv;
    struct display_timing config;
}  module = {
     .hdmi_fc  = HDMI_FC,
     .hdmi_mc  = HDMI_MC,
     .tcon_top = TCON_TOP,
     .tcon_tv  = TCON_TV,
};

// using fixed standard timings (should use edid to negotiate with monitor instead?)
static const struct display_timing avail_resolutions[] = {
           //     {horiz}                {vert}            pixel rate, pll_m, pll_n, tcon_n, tcon_m, de_m
    {HDMI_1080P,  {1920,  88,  44, 148}, {1080, 4, 5, 36}, 148500000, 0x62, 1, 0, 1, 3},
    {HDMI_HD,     {1280, 110,  40, 220}, { 720, 5, 5, 20},  74250000, 0x62, 1, 0, 3, 3},
    {HDMI_SVGA,   { 800,  40, 128,  88}, { 600, 1, 4, 23},  40000000, 0x13, 0, 0, 2, 0},
    {HDMI_INVALID, {0}}
};

static bool select_resolution(hdmi_resolution_id_t id);
static void enable_display_clocks(void);
static void hdmi_controller_init(void);
static void tcon_init(void);
static int sun20i_d1_hdmi_phy_config(void);

void hdmi_init(hdmi_resolution_id_t id) {
    if (!select_resolution(id)) {
        error("Unable to init hdmi, resolution id is invalid!\n");
    }
    enable_display_clocks();
    hdmi_controller_init();
    tcon_init();

    // possible to call hdmi_init again to change resolution
    // but must init PHY exactly once
    // (does not need re-init for change in resolution
    // and in fact re-init will cause problems)
    static bool phy_initialized = false;
    if (!phy_initialized) {
        sun20i_d1_hdmi_phy_config();
        phy_initialized = true;
    }
}

static bool select_resolution(hdmi_resolution_id_t id) {
   for (int i = 0; avail_resolutions[i].id != HDMI_INVALID; i++) {
        if (avail_resolutions[i].id == id) {
            module.config = avail_resolutions[i];
            return true;
        }
    }
    return false;
}

hdmi_resolution_id_t hdmi_best_match(int width, int height) {
    hdmi_resolution_id_t chosen = HDMI_INVALID;
    // resolutions listed in order from largest to smallest, choose "tightest" (i.e. smallest that fits)
    for (int i = 0; avail_resolutions[i].id != HDMI_INVALID; i++) {
        if (width <= avail_resolutions[i].horiz.pixels && height <= avail_resolutions[i].vert.pixels) {
            chosen = avail_resolutions[i].id;
        }
    }
    return chosen;
}

int hdmi_get_screen_width(void) {
    return module.config.horiz.pixels;
}
int hdmi_get_screen_height(void) {
    return module.config.vert.pixels;
}

// enable all clocks needed for HDMI+TCON+DE2
static void enable_display_clocks(void) {
    ccu_enable_pll(CCU_PLL_VIDEO0_CTRL_REG, module.config.pll_m, module.config.pll_n);
    // hdmi clock, both sub and main (bits 16 and 17)
    ccu_enable_bus_clk(CCU_HDMI_BGR_REG, 1 << 0, (1 << 16)|(1 << 17) );
    ccu_write(CCU_HDMI_24M_CLK_REG, 1 << 31); 
    // tcon top clock
    ccu_enable_bus_clk(CCU_DPSS_TOP_BGR_REG, 1 << 0, 1 << 16);
    // tcon tv clock
    ccu_enable_bus_clk(CCU_TCONTV_BGR_REG, 1 << 0, 1 << 16);
    uint32_t src = 0; 	// source 00 = PLLVideo0(1x)
    uint32_t N_FACTOR = module.config.tcon_n, M_FACTOR = module.config.tcon_m;
    // bits: ena @[31], src @[25:24] N @[9:8] M @[3:0]
    ccu_write(CCU_TCONTV_CLK_REG, (1 << 31) | (src << 24) | (N_FACTOR << 8) | (M_FACTOR << 0));
    // de clock
    ccu_enable_bus_clk(CCU_DE_BGR_REG, 1 << 0, 1 << 16);
    src = 1; // src 01 = PLLVideo0(4x)
    M_FACTOR = module.config.de_m; // de spec says 250 Mhz
    ccu_write(CCU_DE_CLK_REG, (1 << 31) | (src << 24) | (M_FACTOR << 0));
}

// HDMI controller registers must be written in 8-bit chunks
static void hdmi_write_short(volatile uint8_t arr[], short val) {
    arr[0] = val & 0xff;  
    arr[1] = val >> 8;
}

#define BLANKING(d) (d.front_porch + d.sync_pulse + d.back_porch)
#define TOTAL(d) (d.pixels + BLANKING(d))

static void hdmi_controller_init(void) {
    // doc Synopsys Designware @ https://people.freebsd.org/~gonzo/arm/iMX6-HDMI.pdf

    // frame controller
    // V and H sync polarity bits 5&6, data enble input polarity bit 4, HDMI mode bit 3    
    module.hdmi_fc->regs.invidconf = (1<<6) | (1<<5) | (1<<4); // no HDMI (bit 3) to work with older DVI monitors

    hdmi_write_short(module.hdmi_fc->regs.inhactv, module.config.horiz.pixels);
    hdmi_write_short(module.hdmi_fc->regs.inhblank, BLANKING(module.config.horiz));
    hdmi_write_short(module.hdmi_fc->regs.hsyncindelay, module.config.horiz.front_porch);
    hdmi_write_short(module.hdmi_fc->regs.hsyncinwidth, module.config.horiz.sync_pulse);
    hdmi_write_short(module.hdmi_fc->regs.invactv, module.config.vert.pixels);
    module.hdmi_fc->regs.invblank = BLANKING(module.config.vert);
    module.hdmi_fc->regs.vsyncindelay = module.config.vert.front_porch;
    module.hdmi_fc->regs.vsyncinwidth = module.config.vert.sync_pulse;

    module.hdmi_fc->regs.ctrldur = 12; 	// spacing set at minimums
    module.hdmi_fc->regs.exctrldur = 32;    // values from linux bridge driver
    module.hdmi_fc->regs.exctrlspac = 1;
    module.hdmi_fc->regs.chpream[0] = 0x0b;
    module.hdmi_fc->regs.chpream[1] = 0x16;
    module.hdmi_fc->regs.chpream[2] = 0x21;

    // main controller
    module.hdmi_mc->regs.clkdis = 0x7c; // enable pixel+tdms clock (disable others)
}

static void tcon_init(void) {
    module.tcon_tv->regs.gtcl = (1 << 31);    // tcon_tv global enable @[31]
    // vertical video start delay is computed by excluding vertical front
    // porch value from total vertical timings
    // See https://lkml.iu.edu/hypermail/linux/kernel/1910.0/06574.html
    uint32_t start_delay = TOTAL(module.config.vert) - (module.config.vert.pixels + module.config.vert.front_porch) - 1;
    module.tcon_tv->regs.ctl = (1 << 31) | (start_delay << 4); // enable tv @[31], delay @[8-4] (@[1] set for blue test data)

    // [0] input resolution, [1] upscaled resolution, [2] output resolution
    for (int i = 0; i < 3; i++) {
        module.tcon_tv->regs.dimensions[i].width = module.config.horiz.pixels - 1;
        module.tcon_tv->regs.dimensions[i].height = module.config.vert.pixels - 1;
    };
    module.tcon_tv->regs.htiming.total = TOTAL(module.config.horiz) - 1;
    module.tcon_tv->regs.htiming.bp = module.config.horiz.sync_pulse + module.config.horiz.back_porch - 1;
    module.tcon_tv->regs.vtiming.total = 2 * TOTAL(module.config.vert);
    module.tcon_tv->regs.vtiming.bp = module.config.vert.sync_pulse + module.config.vert.back_porch - 1;
    module.tcon_tv->regs.sync.horiz = module.config.horiz.sync_pulse - 1;
    module.tcon_tv->regs.sync.vert = module.config.vert.sync_pulse - 1;
    
    // configure tcon_top mux and select tcon tv
    // clear state, config for hdmi, enable
    module.tcon_top->regs.gate &= ~((0xf << 28) | (0xf << 20)); // clear bits @[31-28], [23-20]
    module.tcon_top->regs.gate |= (0x1 << 28) | (0x1 << 20); // from Linux
    module.tcon_top->regs.port_sel &= ~((0x3 << 4) | (0x3 << 0)); // clear bits @[5-4], @[1-0]
    module.tcon_top->regs.port_sel |= (2 << 0); // config mixer0 -> tcon_tv
}

/*
 * Code below drives the HDMI PHY used on the Allwinner D1 SoC. The
 * PHY is responsible for low-level HDMI clock and timing signals.
 * The PHY used for the D1 is custom design by Allwinner. Sadly there 
 * seems to be zero documentation for it. I got the code below from the
 * BSP and linux kernel driver.
 *
 * juliez July 2023
 */

// Everything from here down was taken near-verbatim from linux-sunxi kernel driver
// https://github.com/smaeul/linux/blob/d1/all/drivers/gpu/drm/sun4i/sun8i_hdmi_phy.c

#define AW_PHY_TIMEOUT 1000

static int sun20i_d1_hdmi_phy_enable(void) {
    int i = 0, status = 0;

    //enib -> enldo -> enrcal -> encalog -> enbi[3:0] -> enck -> enp2s[3:0] -> enres -> enresck -> entx[3:0]
    phy_base->phy_ctl4.bits.reg_slv = 4;     //low power voltage 1.08V, default is 3, set 4 as well as pll_ctl0 bit [24:26]
    phy_base->phy_ctl5.bits.enib = 1;
    phy_base->phy_ctl0.bits.enldo = 1;
    phy_base->phy_ctl0.bits.enldo_fs = 1;
    phy_base->phy_ctl5.bits.enrcal = 1;

    phy_base->phy_ctl5.bits.encalog = 1;

    for (i = 0; i < AW_PHY_TIMEOUT; i++) {
        timer_delay_us(5);
        status = phy_base->phy_pll_sts.bits.phy_rcalend2d_status;
        if (status & 0x1) {
            break;
        }
    }
    if ((i == AW_PHY_TIMEOUT) && !status) {
        printf("phy_rcalend2d_status TIMEOUT\n");
        return -1;
    }

    phy_base->phy_ctl0.bits.enbi = 0xF;
    for (i = 0; i < AW_PHY_TIMEOUT; i++) {
        timer_delay_us(5);
        status = phy_base->phy_pll_sts.bits.pll_lock_status;
        if (status & 0x1) {
            break;
        }
    }
    if ((i == AW_PHY_TIMEOUT) && !status) {
        printf("pll_lock_status TIMEOUT\n");
        return -1;
    }

    phy_base->phy_ctl0.bits.enck = 1;
    phy_base->phy_ctl5.bits.enp2s = 0xF;
    phy_base->phy_ctl5.bits.enres = 1;
    phy_base->phy_ctl5.bits.enresck = 1;
    phy_base->phy_ctl0.bits.entx = 0xF;

    for (i = 0; i < AW_PHY_TIMEOUT; i++) {
        timer_delay_us(5);
        status = phy_base->phy_pll_sts.bits.tx_ready_dly_status;
        if (status & 0x1) {
            break;
        }
    }
    if ((i == AW_PHY_TIMEOUT) && !status) {
        printf("tx_ready_status TIMEOUT\n");
        return -1;
    }

    return 0;
}

static int sun20i_d1_hdmi_phy_config(void) {
    int ret;

    /* enable all channel */
    phy_base->phy_ctl5.bits.reg_p1opt = 0xF;

    // phy_reset
    phy_base->phy_ctl0.bits.entx = 0;
    phy_base->phy_ctl5.bits.enresck = 0;
    phy_base->phy_ctl5.bits.enres = 0;
    phy_base->phy_ctl5.bits.enp2s = 0;
    phy_base->phy_ctl0.bits.enck = 0;
    phy_base->phy_ctl0.bits.enbi = 0;
    phy_base->phy_ctl5.bits.encalog = 0;
    phy_base->phy_ctl5.bits.enrcal = 0;
    phy_base->phy_ctl0.bits.enldo_fs = 0;
    phy_base->phy_ctl0.bits.enldo = 0;
    phy_base->phy_ctl5.bits.enib = 0;
    phy_base->pll_ctl1.bits.reset = 1;
    phy_base->pll_ctl1.bits.pwron = 0;
    phy_base->pll_ctl0.bits.envbs = 0;

    // phy_set_mpll
    phy_base->pll_ctl0.bits.cko_sel = 0x3;
    phy_base->pll_ctl0.bits.bypass_ppll = 0x1;
    phy_base->pll_ctl1.bits.drv_ana = 1;
    phy_base->pll_ctl1.bits.ctrl_modle_clksrc = 0x0; //0: PLL_video   1: MPLL
    phy_base->pll_ctl1.bits.sdm_en = 0x0;            //mpll sdm jitter is very large, not used for the time being
    phy_base->pll_ctl1.bits.sckref = 0;        //default value is 1
    phy_base->pll_ctl0.bits.slv = 4;
    phy_base->pll_ctl0.bits.prop_cntrl = 7;   //default value 7
    phy_base->pll_ctl0.bits.gmp_cntrl = 3;    //default value 1
    phy_base->pll_ctl1.bits.ref_cntrl = 0;
    phy_base->pll_ctl0.bits.vcorange = 1;

    // phy_set_div
    phy_base->pll_ctl0.bits.div_pre = 0;      //div7 = n+1
    phy_base->pll_ctl1.bits.pcnt_en = 0;
    phy_base->pll_ctl1.bits.pcnt_n = 1;       //div6 = 1 (pcnt_en=0)    [div6 = n (pcnt_en = 1) note that some multiples are problematic] 4-256
    phy_base->pll_ctl1.bits.pixel_rep = 0;    //div5 = n+1
    phy_base->pll_ctl0.bits.bypass_clrdpth = 0;
    phy_base->pll_ctl0.bits.clr_dpth = 0;     //div4 = 1 (bypass_clrdpth = 0)
    //00: 2    01: 2.5  10: 3   11: 4
    phy_base->pll_ctl0.bits.n_cntrl = 1;      //div
    phy_base->pll_ctl0.bits.div2_ckbit = 0;   //div1 = n+1
    phy_base->pll_ctl0.bits.div2_cktmds = 0;  //div2 = n+1
    phy_base->pll_ctl0.bits.bcr = 0;          //div3    0: [1:10]  1: [1:40]
    phy_base->pll_ctl1.bits.pwron = 1;
    phy_base->pll_ctl1.bits.reset = 0;

    // configure phy
    /* config values taken from table */
    phy_base->phy_ctl1.dwval = ((phy_base->phy_ctl1.dwval & 0xFFC0FFFF) | /* config->phy_ctl1 */ 0x0);
    phy_base->phy_ctl2.dwval = ((phy_base->phy_ctl2.dwval & 0xFF000000) | /* config->phy_ctl2 */ 0x0);
    phy_base->phy_ctl3.dwval = ((phy_base->phy_ctl3.dwval & 0xFFFF0000) | /* config->phy_ctl3 */ 0xFFFF);
    phy_base->phy_ctl4.dwval = ((phy_base->phy_ctl4.dwval & 0xE0000000) | /* config->phy_ctl4 */ 0xC0D0D0D);
    //phy_base->pll_ctl0.dwval |= config->pll_ctl0;
    //phy_base->pll_ctl1.dwval |= config->pll_ctl1;

    // phy_set_clk
    phy_base->phy_ctl6.bits.switch_clkch_data_corresponding = 0;
    phy_base->phy_ctl6.bits.clk_greate0_340m = 0x3FF;
    phy_base->phy_ctl6.bits.clk_greate1_340m = 0x3FF;
    phy_base->phy_ctl6.bits.clk_greate2_340m = 0x0;
    phy_base->phy_ctl7.bits.clk_greate3_340m = 0x0;
    phy_base->phy_ctl7.bits.clk_low_340m = 0x3E0;
    phy_base->phy_ctl6.bits.en_ckdat = 1;       //default value is 0

    // phy_base->phy_ctl2.bits.reg_resdi = 0x18;
    // phy_base->phy_ctl4.bits.reg_slv = 3;         //low power voltage 1.08V, default value is 3

    phy_base->phy_ctl1.bits.res_scktmds = 0;  //
    phy_base->phy_ctl0.bits.reg_csmps = 2;
    phy_base->phy_ctl0.bits.reg_ck_test_sel = 0;  //?
    phy_base->phy_ctl0.bits.reg_ck_sel = 1;
    phy_base->phy_indbg_ctrl.bits.txdata_debugmode = 0;

    // phy_enable
    ret = sun20i_d1_hdmi_phy_enable();
    if (ret)
        return ret;

    phy_base->phy_ctl0.bits.sda_en = 1;
    phy_base->phy_ctl0.bits.scl_en = 1;
    phy_base->phy_ctl0.bits.hpd_en = 1;
    phy_base->phy_ctl0.bits.reg_den = 0xF;
    phy_base->pll_ctl0.bits.envbs = 1;

    return 0;
}
