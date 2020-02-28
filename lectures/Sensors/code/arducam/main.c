/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * 
 * Completed on: March 14, 2016
 * Arjun Balasingam, Eric Fritz
 * CS 107e, Final Project
 * Shell to demonstrate camera driver features
 */

#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "arducam.h"
#include "omni.h"
#include "uart.h"
#include "gl.h"
#include "printf.h"

// size of the image coming in
#define WIDTH   320
#define HEIGHT  240

// size of the console, values too large get trimmed later
#define CONSOLE_ROWS	20
#define CONSOLE_COLS	50

//center the image on the screen? 
#define CENTERED	1

#define IMAGE_CAPTURE 0
#define IMAGE_STREAM  1

/* init_peripherals
 * contains all the initialization for the program
 * adjusts the image window and sets the shell parameters
 */
void init_peripherals(void) {
    timer_init();
	timer_delay_ms(100);

	uart_init();

	//set multiplier to 1 for full screen, 2 for half, etc
	unsigned graphics_width = WIDTH*3/2;
	unsigned graphics_height = HEIGHT*3/2;
	unsigned image_start = graphics_width/2 - WIDTH/2;
	
	//initailize graphics
	gl_init(graphics_width, graphics_height, GL_DOUBLEBUFFER);

	//start up the camera, set the start of the image's top left corner in the 
	//graphics display. This starts SPI and I2C inside
	if(CENTERED) arducam_init(WIDTH,HEIGHT,image_start,0);
	else arducam_init(WIDTH,HEIGHT,0,0);
}

/* 
 * comtinuously capture images
 */
void main(void)
{	
	init_peripherals();

	while(1) {
        capture_image();
    }
}

/*
                  ."-,.__
                  `.     `.  ,
               .--'  .._,'"-' \
              /    .'         `'
              `.   /          ,'
                \  '--.   ,-"'
                 `"\   |  \
                    -. \, |
                     `--\.'      ___.
                          \     |._, \
                _.,        `.   <  <\                _
              ,' /           `, `.   | \            ( `
           ../, `.            `  |    |\`.           \ \_
          ,' ,.. |            _._'    ||\|            )  '".
         / ,'   \ \         ,'.-.`-._,'  |           .  _._`.
       ,' /      \ \        `' / `--/   | \          / /   ..\
     .'  /        \ .         |^___^ _ ,'` `        / /     `.`.
     |  '          ..         `-...-"  |  `-'      / /        . \.
     | /           |\__           |    |          / /          `. \
    , /            \   .          |    |         / /             ` `
   / /          ,.  `._ `-_       |    |  _   ,-' /               ` \
  / .           \"`_/\ `-_ \_,.  /'    +-' `-'  _,        ..,-.    \`.
 .  '         .-|    ,`   `    '/       \__.---'     ,   .'   '     \ \
 ' /          `._    |     .' /          \..      ,_| |  `.  ,'`     |`
 |'      _.-""\  \    \ _,'  `            \ `.   `.   -.- / |   |     \\
 ||    ,'      `. `.   '       _,...._        \   /   _/ '  |   \     ||
 ||  ,'          `. ;.,.---' ,'       `.   `.. `-'  .   /_ .'    |_   ||
 || '              V      / /           \   | \   ,'   ,' '.    !  `. ||
 ||/            _,-------7 '             \  |  `-'    |         /    `||
  \|          ,' .-   ,' ||               | .-.        `.      .'     ||
  `'        ,'    `".'    |               |    `.        '. -.'       `'
           /      ,'      |               |,'    \-.._,.'/'
          (      /        .               .       \    .'/
          `.    |         `.             /         :_,'.'
           \ `...\   _     ,'-.        .'         /_.-'
            `-.__ `,  `'   |  _.>----''.  _  __  /
                 .'        /"'          |  "'   '_
                /_ ,  , . \             '.  ,  '.__\
                  /_./"'"\,'              `/ `-.|
*/
