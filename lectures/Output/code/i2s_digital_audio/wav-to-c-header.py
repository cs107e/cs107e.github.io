#!/usr/bin/env python3

# Quick & dirty script to convert wav file => c struct
# juliez Sun Sep  1 08:33:44 PDT 2024
VERSION = 1.1

import datetime, os, struct, sys, wave

def parse_wav(path):
    wav_file = wave.open(path)
    samples_per_frame,bytes_per_sample,frames_per_sec,nframes,_,_ = wav_file.getparams()
    FMT_CODE = {1:'B', 2:'h', 4:'l'}
    fmt = f"={samples_per_frame*FMT_CODE[bytes_per_sample]}"  # fmt of each frame
    samples = []
    for _ in range(nframes):
        samples.extend(struct.unpack(fmt, wav_file.readframes(1))) # decode frame, concat samples
    wav_file.close()
    return frames_per_sec,samples_per_frame,8*bytes_per_sample,samples

def output_data(filename, frames_per_sec,samples_per_frame,bits_per_sample,samples):
    INDENT = '\n    '
    print(f"const struct wav_file_t wav_data = {{", end=INDENT)
    print(f".name= \"{filename}\",", end=INDENT)
    print(f".frames_per_sec= {frames_per_sec}, // Hz", end=INDENT)
    print(f".samples_per_frame= {samples_per_frame}, // 1=mono, 2=stereo", end=INDENT)
    print(f".bits_per_sample= {bits_per_sample},", end=INDENT)
    print(f".min_val= {min(samples)}, .max_val= {max(samples)},", end=INDENT)
    print(f".n_data= {len(samples)},", end=INDENT)
    print(f".data{bits_per_sample} = (int{bits_per_sample}_t[]) {{", end=INDENT)
    for i,sample in enumerate(samples):
        print(f'{sample:6}', end=', ')
        if not ((i+1) % 8): print('', end=INDENT)
    print("},\n};\n")
    print(f"const struct wav_file_t *gwav = &wav_data;")

def output_header(filename):
    print(f"""
/*
 * C header of sound data extracted from a wav file.
 *
 * Input file '{filename}' converted on date { datetime.datetime.today().strftime('%Y-%m-%d')}
 * using version {VERSION} of script {os.path.basename(__file__)}
 */

#pragma once
#include <stdint.h>

#ifndef _WAV_STRUCT
struct wav_file_t {{
    const char *name;
    int frames_per_sec; // sampling frequency in Hz
    int samples_per_frame; // 1=mono, 2=stereo
    int bits_per_sample; // data can be 8, 16, or 32-bit
    int min_val, max_val;
    int n_data;
    union {{
        int8_t  *data8;
        int16_t *data16;
        int32_t *data32;
        void *data;
    }};
}};
#define _WAV_STRUCT
#endif
""")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        output_header(None)
    else:
        path = sys.argv[1]
        wav_tuple = parse_wav(path)
        if len(sys.argv) > 2: sys.stdout = open(sys.argv[2], 'wt')
        filename = os.path.basename(path)
        output_header(filename)
        output_data(filename, *wav_tuple)