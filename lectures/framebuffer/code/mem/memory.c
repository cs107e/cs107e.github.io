#include <stdio.h>

#include "uart.h"
#include "property.h"

void main(void) 
{
    property_init();
    property_add( TAG_GET_ARM_MEMORY );
    property_add( TAG_GET_VC_MEMORY );
    property_request();

    property_t* mp;

    mp = property_get( TAG_GET_ARM_MEMORY );
    if( mp )
        printf( "Arm Memory: %08x %08x\r\n", 
            mp->data.buffer_32[0], mp->data.buffer_32[1] );

    mp = property_get( TAG_GET_VC_MEMORY );
    if( mp )
        printf( "GPU Memory: %08x %08x\r\n", 
            mp->data.buffer_32[0], mp->data.buffer_32[1] );
}
