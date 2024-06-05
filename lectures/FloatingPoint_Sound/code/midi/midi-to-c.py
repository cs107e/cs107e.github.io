#!/usr/bin/env python3

# Chris Gregg, October 2021
# This will convert a type-0 (important!) midi file
# to a C header file with the midi bytes encoded for use
# with the play_midi() function in the midi.c library
# usage:
# python3 midi-to-c.py filename.mid > filename.h

import sys

def main():
    if len(sys.argv) != 2:
        print(f'Usage: {sys.argv[0]} filename.mid')
        quit()

    filename = sys.argv[1]
    with open(filename, "rb") as f:
        data = f.read()

    track_data = parse_header(data)
    parse_data(track_data)

def parse_header(data):
    # first 4 bytes should be "MThd"
    description = data[:4]
    if description != b'MThd':
        print("Not a midi file.")
        quit()
    
    print("uint8_t midi_data[] = {\n")
    print("    // preamble")
    comma_bytes(data[:4], 0)
    # size of chunk is 4 bytes
    chunksize = int.from_bytes(data[4:8], byteorder='big')
    # print(f"chunk size: {chunksize}")
    comma_bytes(data[4:8], 4)

    # format (probably 0) 
    midiformat = int.from_bytes(data[8:10], byteorder="big")
    comma_bytes(data[8:10], 8)
    # print(f"midi format: {midiformat}")
    
    # tracks (probably 1) 
    num_tracks = int.from_bytes(data[10:12], byteorder="big")
    comma_bytes(data[10:12], 10)
    # print(f"number of tracks: {num_tracks}")
    
    # division, if bit 15 is low, we have ticks per quarter note
    division = int.from_bytes(data[12:14], byteorder="big")
    comma_bytes(data[12:14], 12)

    mtrk = data[14:18]
    if mtrk != b'MTrk':
        print("No tracks.")
        quit()
    comma_bytes(data[14:18], 14)

    # size of chunk is 4 bytes
    chunksize = int.from_bytes(data[18:22], byteorder='big')
    comma_bytes(data[18:22], 18)
    # print(f"chunk size: {chunksize}")
    print()
    track_data = data[22:]
    return track_data 

def parse_data(data):
    print("\n    // Data:")
    while len(data) > 0:
        # read an event
        delta_time, time_bytes_read, data = read_var_num(data)
        comma_bytes(time_bytes_read, 0);
        print(f" // delta-time ({delta_time})")
        desc = data[0]
        if desc == 0xff:
            # meta data
            meta_type = data[1]
            length, bytes_read, data = read_var_num(data[2:])
            message = [desc, meta_type] + bytes_read + list(data[:length])
            data = data[length:]
            print("    // meta data")
            comma_bytes(message, 0)
            print()
        elif desc == 0xf0:
            # sysex event
            length, bytes_read, data = read_var_num(data[2:])
            message = [desc] + bytes_read + data[:length]
            data = data[length:]
            print("    // sysex")
            comma_bytes(message, 0)
            print()
        elif (desc >> 4) == 0x9 or (desc >> 4) == 0x8:
            # note on/off
            note = data[1]
            velocity = data[2]
            print(f"    {hex(desc)}, {hex(note)}, {hex(velocity)}, // note on/off")
            data = data[3:]
        elif (desc >> 4) == 0xb:
            # control change
            ctrl_num = data[1]
            new_val = data[2]
            print(f"    {hex(desc)}, {hex(ctrl_num)}, {hex(new_val)}, // control change")
            data = data[3:]
        elif (desc >> 4) == 0xc:
            # program change
            new_prog_num = data[1]
            print(f"    {hex(desc)}, {hex(new_prog_num)}, // program change")
            data = data[2:]
        elif (desc >> 4) == 0xe:
            # Pitch Wheel
            part1 = data[1]
            part2 = data[2]
            print(f"    {hex(desc)}, {hex(part1)}, {hex(part2)}, // pitch wheel")
            data = data[3:]
        else:
            print('    ', end='')
            print(f"    // unknown message: {hex(desc)}")
            data = data[1:]
        print()
    print('\n};\n')

def read_var_num(data):
    # returns the num, the bytes read for the num, and the remaining data
    num = 0
    pos = 0
    bytes_read = []
    while True:
        b = data[pos]
        bytes_read.append(b)
        num = (num << 7) + (b & 0x7f)
        pos += 1
        if b & 0x80 == 0:
            break
    return num, bytes_read, data[pos:]

def comma_bytes(data, start_pos):
    # print comma-separated bytes with
    # four spaces at start_pos % 8 == 0,
    # and a newline after (start_pos + 1) % 8 == 0
    pos = start_pos
    for b in data:
        if pos % 8 == 0:
            print('    ', end='')
        print(f'{hex(b)}, ', end='')
        if (pos + 1) % 8 == 0:
            print()
        pos += 1
         
if __name__ == "__main__":
    main()
