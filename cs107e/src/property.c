#include <stdarg.h>
#include <stddef.h>

#include "mailbox.h"
#include "property.h"

typedef enum {
    PROPERTY_SIZE = 0,
    PROPERTY_COMMAND = 1,
} property_offset_t;

typedef enum {
    PROPERTY_REQUEST = 0,
    PROPERTY_RESPONSE = 1,
} property_command_t;

typedef enum {
    TAG = 0,
    TAG_RESPONSE_SIZE = 1,
    TAG_REQUEST_SIZE = 2,
    TAG_VALUE = 3,
} tag_offset_t;

static void memcpy(unsigned char *dst, unsigned char *src, int n) {
    for (int i = 0; i < n; i++) {
        *dst++ = *src++;
    }
}

/* Make sure the property tag buffer is aligned to a 16-byte boundary because
   we only have 28-bits available in the property interface protocol to pass
   the address of the buffer to the VC. */
static int pt[8192] __attribute__((aligned(16)));
static int pti = 0;


void property_init(void) {
    pt[PROPERTY_SIZE] = 0;
    pt[PROPERTY_COMMAND] = PROPERTY_REQUEST;

    pti = 2;
    pt[pti] = 0;
}

void property_add(tag_t tag, ...) {
    va_list vl;
    va_start(vl, tag);

    pt[pti++] = tag;

    switch (tag) {
        case TAG_GET_FIRMWARE_VERSION:
        case TAG_GET_BOARD_MODEL:
        case TAG_GET_BOARD_REVISION:
        case TAG_GET_BOARD_MAC_ADDRESS:
        case TAG_GET_BOARD_SERIAL:
        case TAG_GET_ARM_MEMORY:
        case TAG_GET_VC_MEMORY:
        case TAG_GET_DMA_CHANNELS:
            /* Provide an 8-byte buffer for the response */
            pt[pti++] = 8; // response size
            pt[pti++] = 0; // request size
            pti += 2;
            break;

        case TAG_GET_CLOCKS:
        case TAG_GET_COMMAND_LINE:
            /* Provide a 256-byte buffer */
            pt[pti++] = 256; // response size
            pt[pti++] = 0;   // request size
            pti += 256 >> 2;
            break;

        case TAG_ALLOCATE_BUFFER:
            pt[pti++] = 8; // response size
            pt[pti++] = 4; // request size
            pt[pti++] = va_arg(vl, int);
            pti += 1;
            break;

        case TAG_GET_PHYSICAL_SIZE:
        case TAG_SET_PHYSICAL_SIZE:
        case TAG_TEST_PHYSICAL_SIZE:
        case TAG_GET_VIRTUAL_SIZE:
        case TAG_SET_VIRTUAL_SIZE:
        case TAG_TEST_VIRTUAL_SIZE:
        case TAG_GET_VIRTUAL_OFFSET:
        case TAG_SET_VIRTUAL_OFFSET:
            pt[pti++] = 8; // response size
            if ((tag == TAG_SET_PHYSICAL_SIZE) ||
                    (tag == TAG_SET_VIRTUAL_SIZE) ||
                    (tag == TAG_SET_VIRTUAL_OFFSET) ||
                    (tag == TAG_TEST_PHYSICAL_SIZE) ||
                    (tag == TAG_TEST_VIRTUAL_SIZE)) {
                pt[pti++] = 8; // request size
                pt[pti++] = va_arg(vl, int);   /* Width */
                pt[pti++] = va_arg(vl, int);   /* Height */
            } else {
                pt[pti++] = 0; // request size
                pti += 2;
            }
            break;


        case TAG_GET_ALPHA_MODE:
        case TAG_SET_ALPHA_MODE:
        case TAG_GET_DEPTH:
        case TAG_SET_DEPTH:
        case TAG_GET_PIXEL_ORDER:
        case TAG_SET_PIXEL_ORDER:
        case TAG_GET_PITCH:
            pt[pti++] = 4; // response size
            if ((tag == TAG_SET_DEPTH) ||
                    (tag == TAG_SET_PIXEL_ORDER) ||
                    (tag == TAG_SET_ALPHA_MODE)) {
                pt[pti++] = 4; // request size
                pt[pti++] = va_arg(vl, int);
            } else {
                pt[pti++] = 0; // request size
                pti += 1;
            }
            break;

        case TAG_GET_OVERSCAN:
        case TAG_SET_OVERSCAN:
            pt[pti++] = 16; // response size
            if ((tag == TAG_SET_OVERSCAN)) {
                pt[pti++] = 16; // request size
                pt[pti++] = va_arg(vl, int);   /* Top pixels */
                pt[pti++] = va_arg(vl, int);   /* Bottom pixels */
                pt[pti++] = va_arg(vl, int);   /* Left pixels */
                pt[pti++] = va_arg(vl, int);   /* Right pixels */
            } else {
                pt[pti++] = 0; // request size
                pti += 4;
            }
            break;

        default:
            /* Unsupported tags, just remove the tag from the list */
            pti--;
            break;
    }

    /* Make sure the tags are 0 terminated to end the list */
    pt[pti] = 0;

    va_end(vl);
}


int property_request(void) {
    /* Fill in the size of the buffer */
    pt[PROPERTY_SIZE] = 4 * (pti + 1);
    pt[PROPERTY_COMMAND] = PROPERTY_REQUEST;

    mailbox_write(MAILBOX_TAGS_ARM_TO_VC, (unsigned int)pt);
    return mailbox_read(MAILBOX_TAGS_ARM_TO_VC);
}


property_t* property_get(tag_t tag) {
    static property_t property;
    int* tag_buffer = NULL;

    property.tag = tag;

    int i = 2; // first tag position
    while (i < (pt[PROPERTY_SIZE] >> 2)) {
        if (pt[i] == tag) {
            tag_buffer = &pt[i];
            break;
        }

        i += (pt[i + 1] >> 2) + 3;   // i = pt[TAGSIZE]/4+3;
    }

    if (tag_buffer == NULL) {
        return NULL;
    }

    property.byte_length = tag_buffer[TAG_RESPONSE_SIZE] & 0xFFFF;
    memcpy(property.data.buffer_8,
           (unsigned char *)&tag_buffer[TAG_VALUE],
           property.byte_length);

    return &property;
}

