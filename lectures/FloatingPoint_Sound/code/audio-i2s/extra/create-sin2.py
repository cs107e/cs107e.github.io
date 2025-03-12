#!/usr/bin/env python3

import numpy as np
from scipy.io import wavfile

sampleRate = 44100
frequency = 440
length = 5

breakpoint()
t = np.linspace(0, length, sampleRate * length)  #  Produces a 5 second Audio-File
y = np.sin(frequency * 2 * np.pi * t)  #  Has frequency of 440Hz
    
m = np.max(np.abs(y))
print("m", m)

maxint16 = np.iinfo(np.int16).max  # == 2**15-1
print("maxint16", maxint16)

# You have to Normalize the audio
#y = maxint16 * y / m
y = maxint16 * y

# You have to convert to int16, else doesn't work
y = y.astype(np.int16) 

wavfile.write('Sine.wav', sampleRate, y)
