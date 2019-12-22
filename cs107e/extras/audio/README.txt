Courtesy of Anastasios Angelopoulos, CS107e Spring 2017.
An improved version of the audio library provided by the course.
Basically, these allow for arbitrary waveforms to be sent through the PWM
(nothing is precompiled).  I think audio.c works by taking in a function pointer
which returns a processed sample for output at each clock cycle.
There's a sample rate issue with the PWM, but for low sounds, it doesn't matter too much.

