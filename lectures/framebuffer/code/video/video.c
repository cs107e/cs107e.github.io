#include <stdio.h>

#include "uart.h"
#include "property.h"

void main(void) 
{
    property_init();
    property_add( TAG_GET_PHYSICAL_SIZE );
    property_add( TAG_GET_VIRTUAL_SIZE );
    property_add( TAG_GET_DEPTH );
    property_request();

    property_t* mp;

    if( ( mp = property_get( TAG_GET_PHYSICAL_SIZE ) ) )
    {
        printf( "Physical size : %d %d\n",
            mp->data.buffer_32[0], mp->data.buffer_32[1] );
    }

    if( ( mp = property_get( TAG_GET_VIRTUAL_SIZE ) ) )
    {
        printf( "Virtual size: %d %d\n", 
            mp->data.buffer_32[0], mp->data.buffer_32[1] );
    }

    if( ( mp = property_get( TAG_GET_DEPTH ) ) )
    {
        printf( "%d bpp\n", mp->data.buffer_32[0] );
    }
}
