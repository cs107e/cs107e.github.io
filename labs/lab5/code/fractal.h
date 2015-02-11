/*
 *
 * These routines require floating-point.  You need to compile using
 *  -mfloat-abi=soft -march=armv6
 * and you need to link in gcclibc.a: 
 *  [where gcc lives]/arm-none-eabi/4.8.3/fpu/libgcc.a

 * some decent calls:
        simplest_chaos(3, 100);
        simple_chaos(3, 100);
        simple_attractor(1);
*/

typedef unsigned uint_t;
typedef unsigned char uchar_t;

void simple_attractor(double e);
void simple_chaos(int test, int n) ;
void simplest_chaos(int test, int n);
