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
    HDMI_SVGA,
    HDMI_INVALID
} hdmi_resolution_id_t;

void hdmi_init(hdmi_resolution_id_t r);
hdmi_resolution_id_t hdmi_best_match(int width, int height);
int hdmi_get_screen_width(void);
int hdmi_get_screen_height(void);

#endif
