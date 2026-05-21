/* File: fft.c
 * Code By: Gideon Witchel, Jose Nalon
 *
 * This code implements fft. It is based on an implementation by
 * Jose Nalon which can be found at:
 * https://github.com/jnalon/fast-fourier-transform/blob/master/c/fft.c
 * The implementation has been significantly edited to trim down dependencies,
 * increase efficiency, and add an IFFT option.
 *
 * If you want to process live audio at a reasonable sample rate, you will
 * need to enable hardware floats.
 */

#include "fft.h"
/* The only functions needed from a math library are sin(), cos(), and a
 * definition of PI. I used the 3D graphics library's math library for this,
 * but you can use your favorite implementation or implement those functions
 * yourself.
 */
#include "math.h"

// For fft, call with twiddle = -PI and rescale = 0
// For ifft, call with twiddle = PI and rescale = 1
static void iterative_fft_internal(Complex x[], Complex X[], int N, float twiddle, int rescale)
{
    Complex W, Wkn, w;                                 // Twiddle factors;
    int r, l, p, q, step;

    r = -1;
    int n = N;
    while (n) { r++ ; n = n >> 1; }


    for(int k=0; k<N; k++) {
        l = 0;
        int k_tmp = k;                                 // Bit reverse
        for(int i=0; i<r; i++) {                       // Loop on every bit;
            l = (l << 1) + (k_tmp & 1);                // Test less signficant bit and add;
            k_tmp >>= 1;                               // Test next bit;
        }

	X[l] = x[k];                                   //   bit-reversed order;
    }

    Complex two;
    two.r = 2; two.i = 0;

    step = 1;                                          // Auxiliary for computation of twiddle factors;
    for(int k=0; k<r; k++) {
        W.r = cos(twiddle/step);
        W.i = sin(twiddle/step);
        for(int l=0; l<N; l+=2*step) {
            Wkn.r = 1; Wkn.i = 0;
            for(int n=0; n<step; n++) {
                p = l + n;
                q = p + step;
                w.r = X[q].r * Wkn.r - X[q].i * Wkn.i; // Recombine results;
                w.i = X[q].r * Wkn.i + X[q].i * Wkn.r;
                X[q].r = X[p].r - w.r;
                X[q].i = X[p].i - w.i;
                w.r = X[p].r * two.r - X[p].i * two.i;
                w.i = X[p].r * two.i + X[p].i * two.r;
                X[p].r = w.r - X[q].r;
                X[p].i = w.i - X[q].i;
                Complex old_Wkn = Wkn;
                Wkn.r = old_Wkn.r * W.r - old_Wkn.i * W.i;
                Wkn.i = old_Wkn.r * W.i + old_Wkn.i * W.r;
            }
        }
        step <<= 1;
    }

    if (rescale == 1) {
        float scale = 1.0/(float)N;
        for (int i = 0; i < N; i++) {
            X[i].r = X[i].r * scale;
            X[i].i = X[i].i * scale;
        }
    }
}

void iterative_fft(Complex x[], Complex X[], int N) {
	iterative_fft_internal(x, X, N, -PI, 0);
}

void iterative_ifft(Complex x[], Complex X[], int N) {
	iterative_fft_internal(x, X, N, PI, 1);
}

