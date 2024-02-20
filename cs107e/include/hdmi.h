#ifndef HDMI_H
#define HDMI_H

/*
 * Functions for controlling display hardware.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */


/*
 * `hdmi_resolution_id_t` : enum of available HDMI resolutions
 *
 * These are the available resolutions that the HDMI display
 * hardware can be configured for.
 */
typedef enum  {
    HDMI_1080P,     /* 1920 x 1080 */
    HDMI_HD,        /* 1280 x  720 */
    HDMI_SVGA,      /*  800 x  600 */
    HDMI_INVALID
} hdmi_resolution_id_t;

/*
 * `hdmi_init` : Required initializion for HDMI hardware driver
 *
 * Initializes the HDMI hardware driver and configures output screen
 * for the specified resolution.
 *
 * @param res   requested resolution id chosen from enum above
 */
void hdmi_init(hdmi_resolution_id_t res);

/*
 * `hdmi_best_match` : Return the resolution id that is the best
 * match for the desired width x height.
 *
 * Returns the resolution id of best match or HDMI_INVALID if
 * the size cannot be accommodated.
 *
 * @return      resolution id chosen from enum above
 */
hdmi_resolution_id_t hdmi_best_match(int width, int height);

/*
 * `hdmi_get_screen_width`
 *
 * Get the width of screen in pixels.
 *
 * @return    the width in pixels
 */
int hdmi_get_screen_width(void);

/*
 * `hdmi_get_screen_height`
 *
 * Get the height of screen in pixels.
 *
 * @return    the height in pixels
 */
int hdmi_get_screen_height(void);

#endif
