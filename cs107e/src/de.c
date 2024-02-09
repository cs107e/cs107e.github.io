/*
 * Breaking out DisplayEngine from HDMI
 * 
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Feb 2024
 */

#include "de.h"
#include "assert.h"
#include "hdmi.h"
#include "printf.h"

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
        // we are currently only using pipe1
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
_Static_assert(&(DE_BLD0->regs.pipe[0].offset) == (uint32_t *)0x510100c, "de blender0 pipe[0] offset reg must be at address 0x510100c");
_Static_assert(&(DE_UI_CH1->regs.top_haddr)    == (uint32_t *)0x5103080, "de ui ch1 topaddr reg must be at address 0x5103080");
_Static_assert(&(DE_SCALER->regs.horiz_step)   == (uint32_t *)0x5140088, "de scaler horiz step reg must be at address 0x5140088");

static struct {
    volatile de_t *de;
    volatile de_mixer_t *de_mixer0;
    volatile de_blender_t *de_bld0;
    volatile de_ui_t *de_ui_ch1;
    volatile de_scaler_t *de_scaler;
} const module = {
     .de        = DE_BASE,
     .de_mixer0 = DE_MIXER0,
     .de_bld0   = DE_BLD0,
     .de_ui_ch1 = DE_UI_CH1,
     .de_scaler = DE_SCALER,
};

static void configure_for_fb_requested_size(int fb_width, int fb_height);

// Simplest possible init of DE2 to init and config mixer/blender/ui layer
// Key references:
//      DisplayEngine 2.0 spec https://linux-sunxi.org/images/7/7b/Allwinner_DE2.0_Spec_V1.0.pdf
//      https://linux-sunxi.org/DE2_Register_Guide
void de_init(int width, int height) {
    hdmi_resolution_id_t id = hdmi_best_match(width, height);
    hdmi_init(id);

    // top-level reset
    module.de->regs.ahb_reset = module.de->regs.sclk_gate = module.de->regs.hclk_gate = 1;  // 1 to ungate mixer0

    // config for hdmi full screen resolution
    de_size_t full_screen = {.width= hdmi_get_screen_width() - 1, .height= hdmi_get_screen_height() - 1};

    // config mixer0 (blender)
    module.de_mixer0->regs.glb_ctl = 1; // enable mixer 0
    #warning TODO TEMPORARY: setting blender background to magenta
    printf("TEMPORARY: setting blender background to magenta\n");
    module.de_bld0->regs.background_color = 0xff00ff;
    module.de_mixer0->regs.glb_size = full_screen;
    module.de_bld0->regs.output_size = full_screen;

    // config pipe1, route for first ui layer (ch1)
    uint32_t pipe_index = 1;
    module.de_bld0->regs.pipe_ctrl = ((1 << pipe_index) << 8); // enable pipe 1
    module.de_bld0->regs.pipe[pipe_index].input_size = full_screen;
    module.de_bld0->regs.route = 0x3210; // channels 0-3, each channel route to pipe at same index

    // config first ui layer (ch1)
    uint32_t format = 4; // XRGB_8888
    // default alpha @[24], top-addr-only @[23] no premul @[16] format @[8] enable fill @[4] use global alpha @[1] enable @[0]
    uint32_t features = (0xff << 24) | (0 << 23) | (0 << 16) | (format << 8) | (1 << 4)  | (1 << 1) | (1 << 0);
    module.de_ui_ch1->regs.layer[0].attr_ctrl = features;
    module.de_ui_ch1->regs.layer[0].size = full_screen;
    module.de_ui_ch1->regs.layer[0].offset = 0; // @ top left corner
    module.de_ui_ch1->regs.layer[0].pitch_nbytes = hdmi_get_screen_width() * 4; // 4 bytes (32 bits) per pixel
    #warning TODO TEMPORARY: setting ui layer background to yellow
    printf("TEMPORARY: setting ui layer background to yellow\n");
    module.de_ui_ch1->regs.layer[0].fill_color = 0xffff00;
    module.de_ui_ch1->regs.overlay_size = full_screen;
    module.de_scaler->regs.ctrl = 0;    // disable scaler

    configure_for_fb_requested_size(width, height);   // will center on screen and apply scaler if necessary
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

static void configure_for_fb_requested_size(int fb_width, int fb_height) {
    de_size_t fb_size = {.width= fb_width - 1, .height= fb_height - 1};

    module.de_ui_ch1->regs.overlay_size = fb_size;
    module.de_ui_ch1->regs.layer[0].size = fb_size;
    module.de_ui_ch1->regs.layer[0].pitch_nbytes = fb_width * 4; // 4 bytes (32 bits) per pixel

    // when fb size != hdmi resolution, employ ui scaler
    int screen_width = hdmi_get_screen_width(), screen_height = hdmi_get_screen_height();
    assert(fb_width <= screen_width && fb_height <= screen_height); // fb must fit on screen

    int output_width, output_height;
    int step = compute_scale_step(fb_width, fb_height, screen_width, screen_height, &output_width, &output_height);
    int margin_x = (screen_width - output_width)/2;
    int margin_y = (screen_height - output_height)/2;
    module.de_bld0->regs.pipe[1].offset = (margin_y << 16) | margin_x; // position in center

    if (output_width == fb_width && output_height == fb_height) {
        module.de_scaler->regs.ctrl = 0;    // disable scaler
        module.de_bld0->regs.pipe[1].input_size = fb_size; //  ui layer is direct input to blender pipe 1
    } else {
        module.de_scaler->regs.ctrl = 1;    // enable scaler
        module.de_scaler->regs.horiz_step = module.de_scaler->regs.vert_step = step;
        de_size_t scaler_output_size = {.width= output_width - 1, .height= output_height - 1};
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

void de_set_active_framebuffer(void *addr) {
    module.de_ui_ch1->regs.layer[0].attr_ctrl &= ~(1 << 4); // disable fill
    uintptr_t full_address = (uintptr_t)addr;
    uint32_t low_addr = full_address & 0xffffffff;
    assert((uintptr_t)low_addr == full_address); // confirm address fits in 32 bits
    module.de_ui_ch1->regs.layer[0].top_laddr = low_addr;
}
