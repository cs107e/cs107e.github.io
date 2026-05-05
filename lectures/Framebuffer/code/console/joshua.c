#include "console.h"
#include "timer.h"

void main(void)
{
    console_init(12, 26, GL_GREEN, GL_BLACK);

    console_printf("GREETINGS PROFESSOR FALKEN\n");
    console_printf("\n");
    timer_delay_ms(2000);
    console_printf("HELLO\n");
    console_printf("\n");
    timer_delay_ms(2000);
    console_printf("A STRANGE GAME\n");
    timer_delay_ms(2000);
    console_printf("THE ONLY WINNING MOVE IS\n");
    timer_delay_ms(2000);
    console_printf("NOT TO PLAY\n");
    console_printf("\n");
    timer_delay_ms(2000);
    console_printf("HOW ABOUT A NICE GAME OF CHESS?\n");
    timer_delay(10);
}

