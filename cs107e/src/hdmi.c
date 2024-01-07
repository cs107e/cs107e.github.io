/*
 *
 * This module configures the HDMI display
 * Written to drive Synopsis DesignWare HDMI TX controller using in AW D1.
 * 
 * Support for classic resolutions: 1080p, 720p, SVGA
 * 
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * July 2023
 */

#include "hdmi.h"
#include "_hdmi_private.h"
#include "assert.h"
#include "ccu.h"
#include "printf.h"
#include "timer.h"

struct display_timing {
    struct {
        uint32_t pixels, front_porch, sync_pulse, back_porch;
    } horiz, vert;
    uint32_t pixel_clock_hz;
    uint32_t pll_m, pll_n; // dividers for PLLVideo0
    uint32_t tcon_n, tcon_m; // dividers for TCONTV
    uint32_t de_m; // divider for DE clock
};

static struct {
    struct display_timing hdmi;
} module;

// use fixed common timings (should use edid to get from monitor instead?)
static bool select_resolution(hdmi_resolution_t res) {
    static const struct display_timing 
                        //     {horiz}                 {vert}     pixel rate, pll_m, pll_n, tcon_n, tcon_m
          hdmi_1080p =  { {1920,  88,  44, 148}, {1080, 4, 5, 36}, 148500000, 0x62, 1, 0, 1, 3},
          hdmi_hd =     { {1280, 110,  40, 220},  {720, 5, 5, 20},  74250000, 0x62, 1, 0, 3, 3}, 
          hdmi_svga =   {  {800,  40, 128,  88},  {600, 1, 4, 23},  40000000, 0x13, 0, 0, 2, 0};
    switch(res) {
        case HDMI_1080P:  module.hdmi = hdmi_1080p; return true;
        case HDMI_HD:     module.hdmi = hdmi_hd;    return true;
        case HDMI_SVGA:   module.hdmi = hdmi_svga;  return true;
        default:          return false;
    }
}

// enable all clocks needed for HDMI+TCON+DE2
static void enable_display_clocks(void) {
    ccu_enable_pll(CCU_PLL_VIDEO0_CTRL_REG, module.hdmi.pll_m, module.hdmi.pll_n);
    // hdmi clock, both sub and main (bits 16 and 17)
    ccu_enable_bus_clk(CCU_HDMI_BGR_REG, 1 << 0, (1 << 16)|(1 << 17) );
    ccu_write(CCU_HDMI_24M_CLK_REG, 1 << 31); 
    // tcon top clock
    ccu_enable_bus_clk(CCU_DPSS_TOP_BGR_REG, 1 << 0, 1 << 16);
    // tcon tv clock
    ccu_enable_bus_clk(CCU_TCONTV_BGR_REG, 1 << 0, 1 << 16);
    uint32_t src = 0; 	// source 00 = PLLVideo0(1x)
    uint32_t N_FACTOR = module.hdmi.tcon_n, M_FACTOR = module.hdmi.tcon_m;
    assert(N_FACTOR <= 0x3 && M_FACTOR <= 0xf);
    // bits: ena @[31], src @[25:24] N @[9:8] M @[3:0]
    ccu_write(CCU_TCONTV_CLK_REG, (1 << 31) | (src << 24) | (N_FACTOR << 8) | (M_FACTOR << 0));
    // de clock
    ccu_enable_bus_clk(CCU_DE_BGR_REG, 1 << 0, 1 << 16);
    src = 1; // src 01 = PLLVideo0(4x)
    M_FACTOR = module.hdmi.de_m; // de spec says 250 Mhz
    ccu_write(CCU_DE_CLK_REG, (1 << 31) | (src << 24) | (M_FACTOR << 0));
}

int hdmi_get_screen_width(void) {
    return module.hdmi.horiz.pixels;
}
int hdmi_get_screen_height(void) {
    return module.hdmi.vert.pixels;
}

void de_set_active_framebuffer(void *addr) {
    de_ui_ch1->layer[0].attr_ctrl &= ~(1 << 4); // disable fill
    uintptr_t full_address = (uintptr_t)addr;
    uint32_t low_addr = full_address & 0xffffffff;
    assert((uintptr_t)low_addr == full_address); // confirm address fits in 32 bits
    de_ui_ch1->layer[0].top_laddr = low_addr;
}

// From DE2 docs:
// UI scaler supports 1/16x downscale to 32x upscale
// horizontal scaler is 16-phase 4-tap anti-aliasing filter
// vertical scaler is 16-phase linear filter

static int scale_factor(int in, int out) {
    return ((in * 32) + out - 1)/out; // round up to nearest 32nd (minimum scale for DE2)
}

static int compute_scale_step(int in_width, int in_height, int screen_width, int screen_height, int *p_out_width, int *p_out_height) {
    int horiz_f = scale_factor(in_width, screen_width);
    int vert_f = scale_factor(in_height, screen_height);
    // force square pixels, use larger of two scale factors
    int scale_f = (horiz_f > vert_f) ? horiz_f : vert_f;  // in 32nds
    *p_out_width = (in_width*32)/scale_f;
    *p_out_height = (in_height*32)/scale_f;
    return scale_f << 15; // scale factor stored as x.15 fixed point (only 5 bits of fraction used tho')
}

void de_config_framebuffer(int fb_width, int fb_height) {
    struct de_size fb_size = {.width= fb_width - 1, .height= fb_height - 1};

    de_ui_ch1->overlay_size = fb_size;
    de_ui_ch1->layer[0].size = fb_size;
    de_ui_ch1->layer[0].pitch_nbytes = fb_width * 4; // 4 bytes (32 bits) per pixel

    // when size of framebuffer not same as hdmi resolution, employ ui scaler
    int screen_width = module.hdmi.horiz.pixels, screen_height = module.hdmi.vert.pixels;
    if (!screen_width || !screen_height) printf("TEMPORARY: no hdmi_init() before fb_init() \n");
    assert(fb_width <= screen_width && fb_height <= screen_height); // fb must fit on screen

    int output_width, output_height;
    int step = compute_scale_step(fb_width, fb_height, screen_width, screen_height, &output_width, &output_height);
    int margin_x = (screen_width - output_width)/2;
    int margin_y = (screen_height - output_height)/2;
    de_bld0->pipe[1].offset = (margin_y << 16) | margin_x; // position in center

    if (output_width == fb_width && output_height == fb_height) {
        de_scaler->ctrl = 0;    // disable scaler
        de_bld0->pipe[1].input_size = fb_size; //  ui layer is direct input to blender pipe 1
    } else {
        de_scaler->ctrl = 1;    // enable scaler
        de_scaler->horiz_step = de_scaler->vert_step = step;
        struct de_size scaler_output_size = {.width= output_width - 1, .height= output_height - 1};
        de_scaler->input_size = fb_size; // ui layer is input to scaler
        de_scaler->output_size = scaler_output_size;
        de_bld0->pipe[1].input_size = scaler_output_size; // scaler output is input to blender pipe 1

        // UI scaler works line by line, array of coeff controls of 4-tap blend within line (horiz)
        // below I am setting coeffs to 0,0,0,64 to replicate right pixel of each 4-tap (hard cutoff)
        // note that vert is fixed linear scale of taps without configurable control (fuzzy instead of crisp...)
        // if this ends being unacceptable, could switch to video scaler which has controls for both horiz and vert
        for (int i = 0; i < 16; i++) de_scaler->horiz_coeff[i] = 0x40;
        de_scaler->ctrl |= (1 << 4); // apply coefficients
    }
}

// Simplest possible init of DE2 to init and config mixer/blender/ui layer
// Key references:
//      DisplayEngine 2.0 spec https://linux-sunxi.org/images/7/7b/Allwinner_DE2.0_Spec_V1.0.pdf
//      https://linux-sunxi.org/DE2_Register_Guide
static void de2_init(void) {
    // top-level reset
    de->ahb_reset = de->sclk_gate = de->hclk_gate = 1;  // 1 to ungate mixer0

    // config for hdmi full screen resolution
    struct de_size full_screen = {.width= module.hdmi.horiz.pixels - 1, .height= module.hdmi.vert.pixels - 1};

    // config mixer0 (blender)
    de_mixer0->glb_ctl = 1; // enable mixer 0
    #warning TODO TEMPORARY: setting blender background to magenta
    printf("TEMPORARY: setting blender background to magenta\n");
    de_bld0->background_color = 0xff00ff;
    de_mixer0->glb_size = full_screen;
    de_bld0->output_size = full_screen;

    // config pipe1, route for first ui layer (ch1)
    uint32_t pipe_index = 1;
    de_bld0->pipe_ctrl = ((1 << pipe_index) << 8); // enable pipe 1
    de_bld0->pipe[pipe_index].input_size = full_screen;
    de_bld0->route = 0x3210; // channels 0-3, each ch route to same-index pipe

    // config first ui layer (ch1)
    uint32_t format = 4; // XRGB_8888
    // default alpha @[24], top-addr-only @[23] no premul @[16] format @[8] enable fill @[4] use global alpha @[1] enable @[0]
    uint32_t features = (0xff << 24) | (0 << 23) | (0 << 16) | (format << 8) | (1 << 4)  | (1 << 1) | (1 << 0);
    de_ui_ch1->layer[0].attr_ctrl = features;
    de_ui_ch1->layer[0].size = full_screen;
    de_ui_ch1->layer[0].offset = 0; // @ top left corner
    de_ui_ch1->layer[0].pitch_nbytes = module.hdmi.horiz.pixels * 4; // 4 bytes (32 bits) per pixel
    #warning TODO TEMPORARY: setting ui layer background to yellow
    printf("TEMPORARY: setting ui layer background to yellow\n");
    de_ui_ch1->layer[0].fill_color = 0xffff00;
    de_ui_ch1->overlay_size = full_screen;

    de_scaler->ctrl = 0;    // disable scaler
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
    hdmi_fc->invidconf = (1<<6) | (1<<5) | (1<<4); // no HDMI (bit 3) to work with older DVI monitors

    hdmi_write_short(hdmi_fc->inhactv, module.hdmi.horiz.pixels);
    hdmi_write_short(hdmi_fc->inhblank, BLANKING(module.hdmi.horiz));
    hdmi_write_short(hdmi_fc->hsyncindelay, module.hdmi.horiz.front_porch);
    hdmi_write_short(hdmi_fc->hsyncinwidth, module.hdmi.horiz.sync_pulse);
    hdmi_write_short(hdmi_fc->invactv, module.hdmi.vert.pixels);
    hdmi_fc->invblank = BLANKING(module.hdmi.vert);
    hdmi_fc->vsyncindelay = module.hdmi.vert.front_porch;
    hdmi_fc->vsyncinwidth = module.hdmi.vert.sync_pulse;

    hdmi_fc->ctrldur = 12; 	// spacing set at minimums
    hdmi_fc->exctrldur = 32;    // values from linux bridge driver
    hdmi_fc->exctrlspac = 1; 
    hdmi_fc->chpream[0] = 0x0b;
    hdmi_fc->chpream[1] = 0x16;
    hdmi_fc->chpream[2] = 0x21;

    // main controller
    hdmi_mc->clkdis = 0x7c; // enable pixel+tdms clock (disable others)
}

static void tcon_init(void) {
    tcon_tv->gtcl = (1 << 31);    // tcon_tv global enable @[31]
    // vertical video start delay is computed by excluding vertical front
    // porch value from total vertical timings
    // See https://lkml.iu.edu/hypermail/linux/kernel/1910.0/06574.html
    uint32_t start_delay = TOTAL(module.hdmi.vert) - (module.hdmi.vert.pixels + module.hdmi.vert.front_porch) - 1;
    tcon_tv->ctl = (1 << 31) | (start_delay << 4); // enable tv @[31], delay @[8-4] (@[1] set for blue test data)

    // [0] input resolution, [1] upscaled resolution, [2] output resolution
    for (int i = 0; i < 3; i++) {
        tcon_tv->dimensions[i].width = module.hdmi.horiz.pixels - 1;
        tcon_tv->dimensions[i].height = module.hdmi.vert.pixels - 1;
    };
    tcon_tv->htiming.total = TOTAL(module.hdmi.horiz) - 1;
    tcon_tv->htiming.bp = module.hdmi.horiz.sync_pulse + module.hdmi.horiz.back_porch - 1;
    tcon_tv->vtiming.total = 2 * TOTAL(module.hdmi.vert);
    tcon_tv->vtiming.bp = module.hdmi.vert.sync_pulse + module.hdmi.vert.back_porch - 1;
    tcon_tv->sync.horiz = module.hdmi.horiz.sync_pulse - 1;
    tcon_tv->sync.vert = module.hdmi.vert.sync_pulse - 1;
    
    // configure tcon_top mux and select tcon tv
    // clear state, config for hdmi, enable
    tcon_top->gate &= ~((0xf << 28) | (0xf << 20)); // clear bits @[31-28], [23-20]
    tcon_top->gate |= (0x1 << 28) | (0x1 << 20); // from Linux
    tcon_top->port_sel &= ~((0x3 << 4) | (0x3 << 0)); // clear bits @[5-4], @[1-0]
    tcon_top->port_sel |= (2 << 0); // config mixer0 -> tcon_tv
}

static int sun20i_d1_hdmi_phy_config(void);

void hdmi_init(hdmi_resolution_t res) {
    // init phy only once (does not need re-init for change in
    // resolution, re-init can cause problems... )
    static bool phy_initialized = false;

    bool resolution_ok = select_resolution(res);
    assert(resolution_ok);
    enable_display_clocks();
    hdmi_controller_init();
    tcon_init();
    if (!phy_initialized) {
        sun20i_d1_hdmi_phy_config(); // config PHY
        phy_initialized = true;
    }
    de2_init();
}

/*
 * Code below drives the HDMI PHY used on the Allwinner D1 SoC. The
 * PHY is responsible for low-level HDMI clock and timing signals.
 * The PHY used for the D1 is custom design by Allwinner. Sadly there 
 * seems to be zero documentation for it. I took code from the 
 * BSP and linux kernel driver as a starting point and adapted it
 * to fit our needs.
 * 
 * juliez July 2023
 */

// Code below taken from linux-sunxi kernel driver
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
