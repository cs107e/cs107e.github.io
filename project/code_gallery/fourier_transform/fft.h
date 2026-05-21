/* File: fft.h
 * Code By: Gideon Witchel, Jose Nalon
 *
 * This contains the prototypes for an implementation of fft and ifft.
 * It is based off a module by Jose Nalon, which can be found here:
 * From https://github.com/jnalon/fast-fourier-transform/blob/master/c/fft.h
 */

#ifndef __FFT__
#define __FFT__

typedef struct {
    float r; // Real part of the complex number
    float i; // Imaginary part of the complex number
} Complex;

/**
 * Fast Fourier Transform using an iterative in-place decimation in time algorithm. This has
 * O(N log_2(N)) complexity, and since there are less function calls, it will probably be marginally
 * faster than the recursive versions.
 *
 * @param x The vector of which the FFT will be computed. This should always be called with a vector
 *   of a power of two length, or it will fail. No checks on this are made.
 * @param[out] X The vector that will receive the results of the computation. It needs to be
 *   allocated prior to the function call;
 * @param N The number of elements in the vector.
 */
void iterative_fft(Complex x[], Complex X[], int N);

void iterative_ifft(Complex x[], Complex X[], int N);

#endif
