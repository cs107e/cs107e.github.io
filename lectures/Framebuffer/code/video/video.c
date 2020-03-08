#include "gpio.h"
#include "uart.h"
#include "property.h"
#include "printf.h"

void main(void) {
    gpio_init();
    uart_init();

    property_init();
    property_add(TAG_GET_ARM_MEMORY);
    property_add(TAG_GET_VC_MEMORY);
    property_add(TAG_GET_PHYSICAL_SIZE);
    property_add(TAG_GET_VIRTUAL_SIZE);
    property_add(TAG_GET_DEPTH);
    property_request();

    property_t* mp;

    if ((mp = property_get(TAG_GET_ARM_MEMORY))) {
        printf("Arm Memory: %08x %08x\r\n",
           mp->data.buffer_32[0], mp->data.buffer_32[1]);
    }

    if ((mp = property_get(TAG_GET_VC_MEMORY))) {
        printf("GPU Memory: %08x %08x\r\n",
            mp->data.buffer_32[0], mp->data.buffer_32[1]);
    }

    if ((mp = property_get(TAG_GET_PHYSICAL_SIZE))) {
        printf("Physical size : %d %d\n",
               mp->data.buffer_32[0], mp->data.buffer_32[1]);
    }

    if ((mp = property_get(TAG_GET_VIRTUAL_SIZE))) {
        printf("Virtual size: %d %d\n",
               mp->data.buffer_32[0], mp->data.buffer_32[1]);
    }

    if ((mp = property_get(TAG_GET_DEPTH))) {
        printf("%d bpp (bits per pixel)\n", mp->data.buffer_32[0]);
    }

    uart_putchar(EOT);
}
