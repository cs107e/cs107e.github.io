#ifndef MAILBOX_H
#define MAILBOX_H
/*
 * Interface to Raspberry Pi mailboxes for interacting with on-chip
 * peripherals such as the framebuffer and touchscreen.
 *
 * You read the implementation of this module in Lab 6, answering
 * some questions about how and why it works.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Jan 31 2016
 */

/*
 * The maximum number of channels on the Pi
 */
#define MAILBOX_MAXCHANNEL 16

/*  
 * These constants define mailbox channels. Messages can be sent on
 * these channels. The only channel we use in this course is the 
 * MAILBOX_FRAMEBUFFER channel.
 */ 
typedef enum {
    MAILBOX_POWER_MANAGEMENT = 0,
    MAILBOX_FRAMEBUFFER,
    MAILBOX_VIRTUAL_UART,
    MAILBOX_VCHIQ,
    MAILBOX_LEDS,
    MAILBOX_BUTTONS,
    MAILBOX_TOUCHSCREEN,
    MAILBOX_UNUSED,
    MAILBOX_TAGS_ARM_TO_VC,
    MAILBOX_TAGS_VC_TO_ARM,
} mailbox_channel_t;

/*
 * Write a mailbox message to `channel`
 *
 * @param channel send the message to `channel`
 * @param addr    `addr` is the address of the message data to send. This address
 *                must be a multiple of 16 (i.e. lower 4 bits are 0). 
 *                GPU_NOCACHE will be added to the address so that the contents
 *                of the message is not cached by the GPU.
 */
void mailbox_write(unsigned int channel, unsigned int addr);

/*
 * Receive a mailbox message for `channel`.
 * 
 * @param channel  receive a message on `channel`
 * @return         the data in the message received
 *
 */
unsigned int mailbox_read(unsigned int channel);

#endif
