#include <stdint.h>
#include "audio.h"
#include "AKWF_violin_0001.h"
#include "sin8.h"

#define PHASE (3<<24)

void main ()
{
    audio_init();
    //audio_write_i16(resampled, PHASE);
    audio_write_u8(sinewave, PHASE);
}
