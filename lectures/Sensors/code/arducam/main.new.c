/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * 
 * Completed on: March 14, 2016
 * Arjun Balasingam, Eric Fritz
 * CS 107e, Final Project
 * Shell to demonstrate camera driver features
 */

#include "uart.h"
#include "spi.h"
#include "i2c.h"
#include "omni.h"
#include "arducam.h"

// size of the image coming in          
#define WIDTH   320
#define HEIGHT  240   

/* init_peripherals
 * contains all the initialization for the program
 * adjusts the image window and sets the shell parameters
 */
void init_peripherals(void) {
	uart_init();

	//start up the camera, set the start of the image's top left corner in the 
	//graphics display. This starts SPI inside
	arducam_init(WIDTH,HEIGHT,0,0);
    
	//start up I2C
	i2c_init();
}

/* 
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
