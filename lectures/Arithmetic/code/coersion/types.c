#include <stdint.h>

void main(void)
{
    int8_t  i8  = -1;
    int16_t i16 = i8;
    int32_t i32 = i8;
    int64_t i64 = i32;

    uint8_t  u8  = 1;
    uint16_t u16 = u8;
    uint32_t u32 = u8;
    uint64_t u64 = u32;

    (void)i16;
    (void)i64;
    (void)u16;
    (void)u64;
}
