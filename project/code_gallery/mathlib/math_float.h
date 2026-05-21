// https://www.tutorialspoint.com/c_standard_library/math_h.htm

// double has 24 bits mantissa
// precision is log_10(2^24) = 7.2
#define PI 3.14159265
#define E 2.71828182
#define LN_2 0.69314718
#define SQRT_2 1.41421356

#define NAN (0.0 / 0.0)
#define POS_INF (1.0 / 0.0)
#define NEG_INF (-1.0 / 0.0)

float acos(float x);
// Returns the arc cosine of x in radians.

float asin(float x);
// Returns the arc sine of x in radians.

float atan(float x);
// Returns the arc tangent of x in radians.

float atan2(float y, float x);
// Returns the arc tangent in radians of y/x based on the signs of both values
// to determine the correct quadrant.

float cos(float x);
// Returns the cosine of a radian angle x.

float cosh(float x);
// Returns the hyperbolic cosine of x.

float sin(float x);
// Returns the sine of a radian angle x.

float sinh(float x);
// Returns the hyperbolic sine of x.

float tan(float x);
// Returns the tangent of a radian angle x.

float tanh(float x);
// Returns the hyperbolic tangent of x.

float exp(float x);
// Returns the value of e raised to the xth power.

float frexp(float x, int *exponent);
// The returned value is the mantissa and the integer pointed to by exponent is
// the exponent. The resultant value is x = mantissa * 2 ^ exponent.

float ldexp(float x, int exponent);
// Returns x multiplied by 2 raised to the power of exponent.

float log(float x);
// Returns the natural logarithm (base-e logarithm) of x.

float log10(float x);
// Returns the common logarithm (base-10 logarithm) of x.

float modf(float x, float *integer);
// The returned value is the fraction component (part after the decimal), and
// sets integer to the integer component.

float pow(float x, float n);
// Returns x raised to the power of y.

float sqrt(float x);
// Returns the square root of x.

float ceil(float x);
// Returns the smallest integer value greater than or equal to x.

float fabs(float x);
// Returns the absolute value of x.

float floor(float x);
// Returns the largest integer value less than or equal to x.

float fmod(float x, float y);
// Returns the remainder of x divided by y.