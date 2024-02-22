/*
 * Module to control DisplayEngine 2.0 peripheral on Mango Pi
 * 
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Feb 2024
 */

#include "de.h"
#include "assert.h"
#include "timer.h"

// Display Engine 2.0

typedef struct {
    uint32_t width:13;
    uint32_t resA:3;
    uint32_t height:13;
    uint32_t resB:3;
} de_size_t;

// minimal layout of DE device reigsters to init & configure mixer/blender/ui layer
typedef union {
    struct {
        uint32_t sclk_gate;
        uint32_t hclk_gate;
        uint32_t ahb_reset;
        uint32_t sclk_div; // 4 bits per module
        uint32_t de2tcon_mux; // @[0] swap mixer/tcon
    } regs;
} de_t;

typedef union {
    struct {
        uint32_t glb_ctl;
        uint32_t glb_sts;
        uint32_t glb_dbuffer;
        de_size_t glb_size;
    } regs;
} de_mixer_t;

typedef union {
    struct {
        uint32_t pipe_ctrl;
        struct {
            uint32_t fill_color;
            de_size_t input_size;
            uint32_t offset;
            uint32_t reserved;
        } pipe[4];
        // not sure re: # of pipes? (D1 manual says 4 in one place and 2 in another)
        // we are currently only using pipe [1]
        uint32_t reserved[15];
        uint32_t route;
        uint32_t premultiply;
        uint32_t background_color;
        de_size_t output_size;
    } regs;
} de_blender_t;

typedef union {
    struct {
        struct {
            uint32_t attr_ctrl;
            de_size_t size;
            uint32_t offset;
            uint32_t pitch_nbytes;
            uint32_t top_laddr;
            uint32_t bot_laddr;
            uint32_t fill_color;
            uint32_t reserved;
        } layer[4];
        uint32_t top_haddr;
        uint32_t bot_haddr;
        de_size_t overlay_size;
    } regs;
} de_ui_t;

typedef union {
    struct {
        uint32_t ctrl;
        uint32_t status;
        uint32_t field_ctrl;
        uint32_t bist;
        uint32_t reservedA[12];
        de_size_t output_size;
        uint32_t reservedB[15];
        de_size_t input_size;
        uint32_t reservedC;
        uint32_t horiz_step;
        uint32_t vert_step;
        uint32_t horiz_phase;
        uint32_t reservedD;
        uint32_t vert_phase[2];
        uint32_t reservedE[88];
        uint32_t horiz_coeff[16];
    } regs;
} de_scaler_t;

#define DE_BASE          ((de_t *)0x5000000)
#define DE_MIXER0  ((de_mixer_t *)0x5100000)
#define DE_BLD0  ((de_blender_t *)0x5101000)
#define DE_UI_CH1     ((de_ui_t *)0x5103000)
#define DE_SCALER ((de_scaler_t *)0x5140000)

_Static_assert(&(DE_BASE->regs.de2tcon_mux)    == (uint32_t *)0x5000010, "de de2tcon_mux reg must be at address 0x5000010");
_Static_assert(&(DE_MIXER0->regs.glb_dbuffer)  == (uint32_t *)0x5100008, "de mixer0 glb_dbuffer reg must be at address 0x5100008");
_Static_assert(&(DE_BLD0->regs.pipe[1].offset) == (uint32_t *)0x510101c, "de blender0 pipe[1] offset reg must be at address 0x510101c");
_Static_assert(&(DE_UI_CH1->regs.top_haddr)    == (uint32_t *)0x5103080, "de ui ch1 topaddr reg must be at address 0x5103080");
_Static_assert(&(DE_SCALER->regs.horiz_step)   == (uint32_t *)0x5140088, "de scaler horiz step reg must be at address 0x5140088");

static struct {
    volatile de_t *de;
    volatile de_mixer_t * de_mixer0;
    volatile de_blender_t * de_bld0;
    volatile de_ui_t * de_ui_ch1;
    volatile de_scaler_t * de_scaler;
} const module = {
     .de        = DE_BASE,
     .de_mixer0 = DE_MIXER0,
     .de_bld0   = DE_BLD0,
     .de_ui_ch1 = DE_UI_CH1,
     .de_scaler = DE_SCALER,
};

static void de_config_mixer0(de_size_t full_screen);
static void de_config_blender0(de_size_t full_screen);
static void de_config_ui_ch1(de_size_t fb_size, de_size_t full_screen);
static void de_config_ui_scaler(de_size_t fb_size, de_size_t full_screen);

// Simplest possible init of DE2 to config for dispay of single framebuffer
// Key references:
//      DisplayEngine 2.0 spec https://linux-sunxi.org/images/7/7b/Allwinner_DE2.0_Spec_V1.0.pdf
//      https://linux-sunxi.org/DE2_Register_Guide
void de_init(int fb_width, int fb_height, int screen_width, int screen_height) {
    // top-level reset, ungate clocks
    module.de->regs.ahb_reset = module.de->regs.sclk_gate = module.de->regs.hclk_gate = 1;  // 1 to ungate mixer0

    if (fb_width > screen_width || fb_height > screen_height)
        error("de_init(): requested framebuffer size does not fit on screen");

   // de_size_t registers are slightly wacky: actual width/height = (stored value + 1)
    de_size_t full_screen = {.width= screen_width-1, .height= screen_height-1};
    de_size_t fb_size = {.width= fb_width-1, .height= fb_height-1};

    de_config_mixer0(full_screen);
    de_config_blender0(full_screen);
    de_config_ui_ch1(fb_size, full_screen);
}

void de_set_active_framebuffer(void *addr) {
    module.de_ui_ch1->regs.layer[0].attr_ctrl &= ~(1 << 4); // disable fill
    uintptr_t full_address = (uintptr_t)addr;
    uint32_t low_addr = full_address & 0xffffffff;
    assert((uintptr_t)low_addr == full_address); // confirm address fits in 32 bits
    module.de_ui_ch1->regs.layer[0].top_laddr = low_addr;
    timer_delay_ms(10);  // resync delay
}

// DE Mixer block is a pipeline: framebuffer(s) -> overlay channel(s) -> (optional scaler) -> blender -> output to TCON
// Mixer-0 more full featured (1 video channel, 3 UI overlay)
// Mixer-1 only has 1 video + 1 UI
static void de_config_mixer0(de_size_t full_screen) {
    module.de_mixer0->regs.glb_ctl = 1; // enable mixer 0
    module.de_mixer0->regs.glb_size = full_screen;
}

// DE Blender pairwise composites 2 overlay channels together. Three separate blenders allow blending 4 channels.
// We use only blender 0 with single UI channel.
static void de_config_blender0(de_size_t full_screen) {
    // #warning TODO TEMPORARY: setting blender background to magenta
    // module.de_bld0->regs.background_color = 0xff00ff;
    module.de_bld0->regs.output_size = full_screen;
    uint32_t pipe_index = 1;  // use pipe index 1, route for first ui layer (ch1)
    module.de_bld0->regs.pipe_ctrl = ((1 << pipe_index) << 8); // enable pipe index 1
    module.de_bld0->regs.pipe[pipe_index].input_size = full_screen;
    module.de_bld0->regs.route = 0x3210; // channels 0-3, each channel routed to pipe at corresponding index
}

enum format_t
   { ARGB_8888 = 0x0, ABGR_8888 = 0x1, RGBA_8888 = 0x2, BGRA_8888 = 0x3,
     XRGB_8888 = 0x4, XBGR_8888 = 0x5, RGBX_8888 = 0x6, BGRX_8888 = 0x7,
      RGB_888  = 0x8,  BGR_888  = 0x9,  RGB_565  = 0xa,  BGR_565  = 0xb,
     ARGB_4444 = 0xc, ABGR_4444 = 0xd, RGBA_4444 = 0xe, BGRA_4444 = 0xf };

// DE UI Overlay represents a single framebuffer. Mixer0 has three UI overlay channels, we use only channel 1.
// An optional UI scaler can be used to up/downscale from framebuffer input on route to blender.
static void de_config_ui_ch1(de_size_t fb_size, de_size_t full_screen) {
    // default alpha @[24], top-addr-only @[23] no premul @[16] format @[8] enable fill @[4] use global alpha @[1] enable @[0]
    uint32_t features = (0xff << 24) | (0 << 23) | (0 << 16) | (XRGB_8888 << 8) | (1 << 4)  | (1 << 1) | (1 << 0);
    module.de_ui_ch1->regs.layer[0].attr_ctrl = features;
    module.de_ui_ch1->regs.layer[0].size = fb_size;
    module.de_ui_ch1->regs.layer[0].offset = 0; // position @ top left corner of output
    module.de_ui_ch1->regs.layer[0].pitch_nbytes = (fb_size.width+1) * 4; // 4 bytes (32 bits) per pixel
    module.de_ui_ch1->regs.overlay_size = fb_size;
    // #warning TODO TEMPORARY: setting ui layer background to yellow
    // module.de_ui_ch1->regs.layer[0].fill_color = 0xffff00;
    de_config_ui_scaler(fb_size, full_screen);   // will center on screen and apply scaler if necessary
}

// From DE2 docs:
// UI scaler supports 1/16x downscale to 32x upscale
// horizontal scaler is 16-phase 4-tap anti-aliasing filter
// vertical scaler is 16-phase linear filter

// return ratio in/out as count of 32nds (round up)
static int scale_factor(int in, int out) {
    return ((in * 32) + out - 1)/out; // round up to nearest 32nd (minimum scale for DE2)
}

static int compute_scale_step(de_size_t fb_size, de_size_t full_screen, de_size_t *p_scaled_size, unsigned int *p_offset) {
    int screen_width = full_screen.width+1;
    int screen_height = full_screen.height+1;
    int fb_width = fb_size.width+1;
    int fb_height = fb_size.height+1;

    int horiz_f = scale_factor(fb_width, screen_width);
    int vert_f = scale_factor(fb_height, screen_height);
    // force square pixels, use larger of two scale factors
    int scale_f = (horiz_f > vert_f) ? horiz_f : vert_f;  // in 32nds
    // JDZ: for now, only apply scaling if at least 2x (scale to partial pixels has poor result)
    // otherwise do not scale, center orig rect on screen as-is
    // revisit later, consider whether to use video scaler instead
    if (scale_f > 16) scale_f = 32;
    int output_width = (fb_width*32)/scale_f;
    int output_height = (fb_height*32)/scale_f;
    *p_scaled_size = (de_size_t){.width= output_width - 1, .height= output_height - 1};
    int margin_x = (screen_width - output_width)/2;
    int margin_y = (screen_height - output_height)/2;
    *p_offset = (margin_y << 16) | margin_x; // position in center
    return scale_f << 15; // scale factor stored as x.15 fixed point (only 5 bits of fraction used tho')
}

// DE UI Scaler used to upscale a framebuffer before feeding into blender pipe
// scaler used when frame buffer size is < 1/2 of screen size
static void de_config_ui_scaler(de_size_t fb_size, de_size_t full_screen) {
    de_size_t scaler_output_size;
    uint32_t center_offset;
    int step = compute_scale_step(fb_size, full_screen, &scaler_output_size, &center_offset);
    module.de_bld0->regs.pipe[1].offset = center_offset; // position in center
    if (step == 0x100000) {
        module.de_scaler->regs.ctrl = 0;    // disable scaler
        module.de_bld0->regs.pipe[1].input_size = fb_size; //  ui layer is direct input to blender pipe 1
    } else {
        module.de_scaler->regs.ctrl = 1;    // enable scaler
        module.de_scaler->regs.horiz_phase = 1 << 20;   // correct for first pixel
        module.de_scaler->regs.horiz_step = module.de_scaler->regs.vert_step = step;
        module.de_scaler->regs.input_size = fb_size; // ui layer is input to scaler
        module.de_scaler->regs.output_size = scaler_output_size;
        module.de_bld0->regs.pipe[1].input_size = scaler_output_size; // scaler output is input to blender pipe 1

        // UI scaler works line by line, array of coeff controls of 4-tap blend within line (horiz)
        // below I am setting coeffs to 0,0,0,64 to replicate right pixel of each 4-tap (hard cutoff)
        // note that vert is fixed linear scale of taps without configurable control (fuzzy instead of crisp...)
        // if this ends being unacceptable, could switch to video scaler which has controls for both horiz and vert
        for (int i = 0; i < 16; i++) module.de_scaler->regs.horiz_coeff[i] = 0x40;
        module.de_scaler->regs.ctrl |= (1 << 4); // apply coefficients
    }
}
