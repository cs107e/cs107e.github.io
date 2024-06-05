#ifndef PLAY_H
#define PLAY_H

struct PCM_METADATA {
    int sample_freq;
    int bits_per_sample;
    int block_alignment;
    int mono;
    int repeat;
};

#endif
