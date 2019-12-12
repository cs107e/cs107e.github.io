#ifndef DEBOUNCE_H
#define DEBOUNCE_H

/*
 * Interface to debounce a GPIO pin.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: Mar 23 2016
 */
void debounce_init(int pin);
int debounce(int pin);

#endif
