/* File: blink_c.c
 * ---------------
 * raw blink rewritten in C. Subroutines, yeah!
 */

void pause(int count) {
    while (count-- != 0) {}
}

void main(void) {
    volatile unsigned int *PB_CFG0 = (unsigned int *)0x2000030;
    volatile unsigned int *PB_DATA = (unsigned int *)0x2000040;

    *PB_CFG0 = 1;   // config as output

    while (1) {
        *PB_DATA = 1;   // on
        pause(0x3f00000);
        *PB_DATA = 0;   // off
        pause(0x3f00000);
    }
}
