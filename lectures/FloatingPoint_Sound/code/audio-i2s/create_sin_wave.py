#!/usr/bin/env python3

# this will create pcm code for a sin wave
# with 16-bit resolution
import math

sample_rate = 44100 # hz
frequency = 440 # hz
bits_per_sample = 16
num_seconds = 5
block_alignment = 2
repeat = 1 

print(f"""#include <stdint.h>

#include "play.h"

struct PCM_METADATA pcm_metadata = {{
    .sample_freq = {sample_rate},
    .bits_per_sample = {bits_per_sample},
    .block_alignment = {block_alignment},
    .mono = 1,
    .repeat = {repeat}
}};

const uint16_t pcm_data[] =
{{
    """, end='')
for i in range(sample_rate * num_seconds):
    value = math.sin(frequency * 2 * math.pi * i * num_seconds / (sample_rate * num_seconds - 1))
    value = int(value * (2**(bits_per_sample - 1) - 1))

    if value >= 0:
        value = hex(value)
    else:
        value = hex(value + (bits_per_sample - 1) ** 2)

    print(f"{value}", end=', ')
    if (i + 1) % 8 == 0:
        print("\n    ", end='')
print("\n};") 


