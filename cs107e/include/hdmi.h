#ifndef HDMI_H
#define HDMI_H

/*
 * Functions for controlling display hardware.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

typedef enum  {
    HDMI_1080P,
    HDMI_HD,
    HDMI_SVGA
} hdmi_resolution_t;

void hdmi_init(hdmi_resolution_t r);
int hdmi_get_screen_width(void);
int hdmi_get_screen_height(void);

// API hack for now, need to tweak hdmi/de/fb relationship
void de_config_framebuffer(int width, int height);
void de_set_active_framebuffer(void *addr);

#endif
