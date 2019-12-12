/*
 * Copyright (c) 2014 Marco Maccaferri and Others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "platform.h"

volatile struct   _dma * DMA  = (struct   _dma *) (PERIPHERAL_BASE + DMA0_BASE);
volatile struct _clock * CLK  = (struct _clock *) (PERIPHERAL_BASE + CM_BASE);
volatile struct   _pwm * PWM  = (struct   _pwm *) (PERIPHERAL_BASE + PWM_BASE);
volatile struct  _gpio * GPIO = (struct  _gpio *) (PERIPHERAL_BASE + GPIO_BASE);
volatile struct   _irq * IRQ  = (struct   _irq *) (PERIPHERAL_BASE + IRQ_BASE);

/* Unhandled exceptions - hang the machine */
__attribute__ ((naked)) void bad_exception() {
    while(1)
        ;
}

__attribute__ ((interrupt ("SWI"))) void interrupt_swi() {

}

__attribute__ ((interrupt ("ABORT"))) void interrupt_prefetch_abort() {

}

__attribute__ ((interrupt ("ABORT"))) void interrupt_data_abort() {

}

void mmio_write(uint32_t reg, uint32_t data) {
    //dmb();
    *(volatile uint32_t *) (reg) = data;
    //dmb();
}

uint32_t mmio_read(uint32_t reg) {
    //dmb();
    return *(volatile uint32_t *) (reg);
    //dmb();
}

void mbox_write(uint8_t channel, uint32_t data) {
    while (mmio_read(MAIL_BASE + MAIL_STATUS) & MAIL_FULL)
        ;
    mmio_write(MAIL_BASE + MAIL_WRITE, (data & 0xfffffff0) | (uint32_t) (channel & 0xf));
}

uint32_t mbox_read(uint8_t channel) {
    while (1) {
        while (mmio_read(MAIL_BASE + MAIL_STATUS) & MAIL_EMPTY)
            ;

        uint32_t data = mmio_read(MAIL_BASE + MAIL_READ);
        uint8_t read_channel = (uint8_t) (data & 0xf);
        if (read_channel == channel) {
            return (data & 0xfffffff0);
        }
    }
}

#define TIMER_CLO       0x20003004

int usleep(useconds_t usec) {
    unsigned int cur_timer = mmio_read(TIMER_CLO);
    unsigned int trigger_value = cur_timer + usec;
    unsigned int rollover;

    if (trigger_value > cur_timer)
        rollover = 0;
    else
        rollover = 1;

    for (;;) {
        cur_timer = mmio_read(TIMER_CLO);
        if (cur_timer < trigger_value) {
            if (rollover) {
                rollover = 0;
            }
        }
        else if (!rollover) {
            break;
        }
    }

    return 0;
}

void register_timer(struct timer_wait * tw, unsigned int usec) {
    unsigned int cur_timer = mmio_read(TIMER_CLO);

    tw->usec = usec;
    tw->rollover = 0;
    tw->trigger_value = 0;

    if (usec > 0) {
        tw->trigger_value = cur_timer + usec;
        if (tw->trigger_value > cur_timer)
            tw->rollover = 0;
        else
            tw->rollover = 1;
    }
}

int compare_timer(struct timer_wait * tw) {
    unsigned int cur_timer = mmio_read(TIMER_CLO);

    if (cur_timer < tw->trigger_value) {
        if (tw->rollover)
            tw->rollover = 0;
    } else if (!tw->rollover) {
        if (tw->usec > 0) {
            tw->trigger_value = cur_timer + tw->usec;
            if (tw->trigger_value > cur_timer)
                tw->rollover = 0;
            else
                tw->rollover = 1;
        }
        return 1;
    }

    return 0;
}

unsigned int sleep(unsigned int seconds) {
    usleep(seconds * 1000000);
    return 0;
}
