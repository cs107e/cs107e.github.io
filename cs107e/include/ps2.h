#ifndef PS2_H
#define PS2_H

/*
 * Module that communicates with a PS2 device such as a keyboard
 * or mouse.
 *
 * Students implement this module in assignments 5 and 7.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include "gpio.h"

/*
 * Type: `ps2_device_t`
 *
 * This typedef gives a nickname to the struct that will be used to represent a
 * single PS2 device. The internal details of the struct (e.g. type and names
 * of struct fields) will be given in the file ps2.c; those details are private
 * to the implementation and not shared in the public interface.
 * Clients of the ps2 module are not privy to the details of ps2_device_t,
 * nor should they be. A client simply holds on to the pointer returned by
 * `ps2_new` and sends that pointer as an argument to `ps2_read`.
 */
typedef struct ps2_device ps2_device_t;

/*
 *  `ps2_new`
 *
 * Initializes a new PS2 device connected to specified clock and data GPIO
 * pins and returns pointer to device.
 *
 * To configure a new PS2 device in your code:
 *
 *     ps2_device_t *dev = ps2_new(KEYBOARD_CLK, KEYBOARD_DATA);
 *
 * Notice that this interface is different from the module_init exposed by
 * other library modules. The new operation can be called more than once
 * in order to create multiple PS2 devices, such as one for a keyboard and
 * another for a mouse. When calling ps2_new, the client saves the
 * returned pointer to use as argument in when calling ps2_read.
 * This is how the client indicates which PS2 device is to be read.
 *
 * @param clock     the id of gpio connected to the clock line of the PS2 device
 * @param data      the id of gpio connected to the data line of the PS2 device
 * @return          pointer to new PS2 device or NULL if failed to create
 *
 * Although `ps2_new` configures the requested clock and data GPIOs
 * to use the internal pull-up resistors, it is recommended to choose GPIO
 * pins whose default state is already pull-up. This avoid timing issues
 * if the device attempts to handshake with the Pi before `ps2_new` has
 * been called.
 */
ps2_device_t *ps2_new(gpio_id_t clock, gpio_id_t data);

/*
 * `ps2_read`
 *
 * Read (blocking) a single scancode from the specifed PS2 device.
 * Bits are read on the falling edge of the clock.
 *
 * Reads 11 bits: 1 start bit, 8 data bits, 1 parity bit, and 1 stop bit
 *
 * Discards and restarts the scancode if:
 *   (lab5) The start bit is incorrect
 *   (assign5) or if parity or stop bit is incorrect
 *
 * Returns the 8 data bits of a well-formed PS2 scancode.
 * Will not return until it reads a complete and valid scancode.
 *
 * @param dev     PS2 device from which to read
 * @return        scancode read from PS2 device
 */
unsigned char ps2_read(ps2_device_t *dev);

/*
 * `ps2_write`: optional extension
 *
 * Write a command scancode to the specifed PS2 device.
 * You do not need to implement this function unless you are
 * doing the mouse extension for assign7.
 *
 * @param dev       PS2 device to which to write
 * @param command   scancode to write
 * @return          true if successful write, false otherwise
 */
bool ps2_write(ps2_device_t *dev, unsigned char command);

/*
 * `ps2_use_interrupts` : reference only
 *
 * The default behavior of `ps2_read` is to read scancode bits
 * by polling the clock GPIO and waiting for a falling edge. An
 * alternate mode would be to register for event detection
 * on the clock GPIO to be notified of each falling edge and use
 * an interrupt handler to read a bit. The interrupt handler would
 * enqueue a scancode to an internal queue to be later
 * dequeued by`ps2_read`.
 *
 * The reference implementation of this module has a switch to
 * change the mode for a ps2 device. The default is read-by-polling.
 * After creating a device with `ps2_new`, the client may call
 8 `ps2_use_interrupts(device)` to change that device into
 * read-by-interrupt mode.
 *
 * The client must also globally enable interrupts at system level for
 * interrupts to be generated.
 *
 * The switchable mode is specific to the reference implementation.
 * The student's ps2 module does not have this function.
 * The initial implementation by student is polling-only (assign5) and
 * later changed to interrupt-only (assign7).
 */
void ps2_use_interrupts(ps2_device_t *dev);

#endif
