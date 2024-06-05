#!/usr/bin/env python3

# Chris Gregg, October 2021
# This will convert an 8000Hz, mono .wav audio file
# to a C header file with the bytes encoded for use
# with the audio_write_u8 function in the audio.c library
# usage:
# python3 wav-to-c.py filename.wav > filename.h

# format from here:
# https://isip.piconepress.com/projects/speech/software/tutorials/production/fundamentals/v1.0/section_02/s02_01_p05.html

import sys

def main():
    if len(sys.argv) != 2:
        print(f'Usage: {sys.argv[0]} filename.wav')
        quit()

    filename = sys.argv[1]
    with open(filename, "rb") as f:
        data = f.read()

    sample_freq, bits_per_sample, block_alignment, music_data, wav_type = parse_header(data)
    parse_data(sample_freq, bits_per_sample, block_alignment, wav_type, music_data)

def parse_header(data):
    print('/*')
    # first 4 bytes should be "RIFF"
    description = data[:4]
    if description != b'RIFF':
        print("Not a wav file.")
        quit()
    
    # size of file is 4 bytes
    filesize = int.from_bytes(data[4:8], byteorder='little')
    print(f"file size (minus header): {filesize}")

    # WAVE description header should be WAVE
    wave_desc = data[8:12]
    print(f"Wave description: {wave_desc}")
    
    # Format description header should be "fmt "
    wave_fmt = data[12:16]
    print(f"Wave format: {wave_fmt}")

    # size of the WAV type format (we'll assume this is 16)
    type_format_size = int.from_bytes(data[16:20], byteorder='little')

    if type_format_size != 16:
        print(f"Incorrect type format size ({type_format_size})")
        quit()
    
    # WAV type format, mono/stereo
    wav_type = int.from_bytes(data[20:22], byteorder='little') # should be 0x01
    print(f"wav_type: {hex(wav_type)}")
    mono_or_stereo = int.from_bytes(data[22:24], byteorder='little') # 1 mono, 2 stereo
    print(f"{'mono' if wav_type == 1 else 'stereo'}")
    sample_freq = int.from_bytes(data[24:28], byteorder='little') 
    print(f"sample frequency: {sample_freq}")
    bytes_per_sec = int.from_bytes(data[28:32], byteorder='little') 
    print(f"bytes_per_sec: {bytes_per_sec}")
    block_alignment = int.from_bytes(data[32:34], byteorder='little') 
    print(f"block alignment: {block_alignment}")
    bits_per_sample = int.from_bytes(data[34:36], byteorder='little') 
    print(f"bits_per_sample: {bits_per_sample}")
   
    # List description header should be "data"
    data_desc = data[36:40]
    next_pos = 40
    print(f"Data description: {data_desc}")
    data_chunk_size = int.from_bytes(data[next_pos:next_pos + 4], byteorder='little')
    print(f"Data chunk size: {data_chunk_size}")
    remaining_data = data[next_pos + 4:]

    # if len(remaining_data) != data_chunk_size:
    #     import pdb;pdb.set_trace()
    #     print("Incorrect data size.")
    #     quit()

    print('*/')
    print()
    return sample_freq, bits_per_sample, block_alignment, remaining_data, wav_type

def parse_data(sample_freq, bits_per_sample, block_alignment, wav_type, data):
    data_len = len(data)
    bytes_per_sample = bits_per_sample // 8
    print('#include "play.h"')

    print("\nstruct PCM_METADATA pcm_metadata = {")

    print(f'    .sample_freq = {sample_freq},')
    print(f'    .bits_per_sample = {bits_per_sample},')
    print(f'    .block_alignment = {block_alignment},')
    print(f'    .mono = {1 if wav_type == 1 else 0},')
    print(f'    .repeat = 0')
    print('};\n')
    print(f'const uint{bits_per_sample}_t pcm_data[] = {{')
    for i in range(data_len // bytes_per_sample):
        if i % 256 == 0:
            print("    // 256 samples")
        if i % 8 == 0:
            print('    ', end='')
        start = i * bytes_per_sample
        end = start + bytes_per_sample
        next_sample = int.from_bytes(data[start:end], byteorder='little')
        # for 16-bit samples, this is a 2s-complement number
        # if it is negative, let's convert to the equivalent negative value
        #if (next_sample >> 15) & 1 == 1:
        #    next_sample = next_sample - (2**16) 
        #next_sample += 2**15
        print(f'{hex(next_sample)}', end=', ')
        # print(f'{int(next_sample)}', end=', ')
        if (i + 1) % 8 == 0:
            print()
    print('\n};\n')

if __name__ == "__main__":
    main()
