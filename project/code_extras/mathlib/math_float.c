// uses only elementary methods
// guarantees none of speed, precision or accuracy
// I didn't read the specs
// does not handle any errors
// might be educational; definitely not practical

#include "math_float.h"

// for more precision: use math.h (the double version), not much slower

// double has 24 bits mantissa
// precision is log_10(2^24) = 7.2
// #define PI 3.141592653589793
// #define E 2.718281828459045
// #define LN_2 0.6931471805599453
// #define SQRT_2 1.414213562373095

// #define NAN (0.0 / 0.0)
// #define POS_INF (1.0 / 0.0)
// #define NEG_INF (-1.0 / 0.0)

float pow_int(float x, int n);
float log_any(float x, float y);

float acos(float x) {
  if (x < 0) {
    return PI - acos(-x);
  }
  if (fabs(x) > 1) {
    return NAN;
  }
  if (x == 0) {
    return PI / 2;
  }
  if (x == -1) {
    return PI;
  }
  if (x == 1) {
    return 0;
  }
  return atan(sqrt(1 - x * x) / x);
}

float asin(float x) {
  if (fabs(x) > 1) {
    return NAN;
  }
  if (x == -1) {
    return -PI / 2;
  }
  if (x == 1) {
    return PI / 2;
  }
  return atan(x / sqrt(1 - x * x));
}

// let tan(a) = x
// arctan((sqrt(1 + x^2) - 1) / x)
// = arctan((sec(a) - 1) / tan(a) * (cos(a) / cos(a)))
// = arctan((1 - cos(a)) / sin(a))
// = arctan((2 * (sin(a/2))^2) / (2 * sin(a/2) * cos(a / 2)))
// = arctan(sin(a/2) / cos(a/2)) = arctan(tan(a/2))
// = a/2 = 1/2 * arctan(x)
#define EPSILON 0.1
float atan(float x) {
  float s = 1;
  while (fabs(x) > EPSILON) {
    x = (sqrt(1 + x * x) - 1) / x;
    s *= 2;
  }
  float approx = x - pow_int(x, 3) / 3 + pow_int(x, 5) / 5 - pow_int(x, 7) / 7 +
                 pow_int(x, 9) / 9 - pow_int(x, 11) / 11 + pow_int(x, 13) / 13 -
                 pow_int(x, 15) / 15 + pow_int(x, 17) / 17 -
                 pow_int(x, 19) / 19;
  return s * approx;
}
#undef EPSILON

float atan2(float y, float x) {
  if (x == 0) {
    if (y == 0) {
      return 0;
    }
    if (y > 0) {
      return PI / 2;
    } else {
      return -PI / 2;
    }
  }
  if (x > 0) {
    return atan(y / x);
  } else {
    if (y >= 0) {
      return atan(y / x) + PI;
    } else {
      return atan(y / x) - PI;
    }
  }
  return 0;
}

float cos(float x) {
  int sign = 1;
  // cos(-a) = cos(a)
  // a in [0, \infinity)
  if (x < 0) {
    x = -x;
  }
  // cos(a + 2k * PI) = cos(a)
  // a in [0, 2 * PI]
  if (x > 2 * PI) {
    x -= (int)(x / 2 / PI) * 2 * PI;
  }
  // cos(PI + a) = -cos(a)
  // a in [0, PI]
  if (x > PI) {
    x -= PI;
    sign *= -1;
  }
  // cos(PI - a) = -cos(a)
  // a in [0, PI / 2]
  if (x > PI / 2) {
    x = PI - x;
    sign *= -1;
  }
  // sin(PI / 2 - a) = cos(a)
  // a in [0, PI / 4]
  if (x > PI / 4) {
    return sign * sin(PI / 2 - x);
  }
  float approx = 1 - pow_int(x, 2) / 2 + pow_int(x, 4) / 24 -
                 pow_int(x, 6) / 720 + pow_int(x, 8) / 40320 -
                 pow_int(x, 10) / 3628800 + pow_int(x, 12) / 479001600 -
                 pow_int(x, 14) / 87178291200;
  return sign * approx;
}

float cosh(float x) { return (exp(x) + exp(-x)) / 2; }

float sin(float x) {
  int sign = 1;
  // sin(-a) = -sin(a)
  // a in [0, \infinity)
  if (x < 0) {
    x = -x;
    sign *= -1;
  }
  // sin(a + 2k * PI) = sin(a)
  // a in [0, 2 * PI]
  if (x > 2 * PI) {
    x -= (int)(x / 2 / PI) * 2 * PI;
  }
  // sin(PI + a) = -sin(a)
  // a in [0, PI]
  if (x > PI) {
    x -= PI;
    sign *= -1;
  }
  // sin(PI - a) = sin(a)
  // a in [0, PI / 2]
  if (x > PI / 2) {
    x = PI - x;
  }
  // cos(PI / 2 - a) = sin(a)
  // a in [0, PI / 4]
  if (x > PI / 4) {
    return sign * cos(PI / 2 - x);
  }
  float approx = x - pow_int(x, 3) / 6 + pow_int(x, 5) / 120 -
                 pow_int(x, 7) / 5040 + pow_int(x, 9) / 362880 -
                 pow_int(x, 11) / 39916800 + pow_int(x, 13) / 6227020800;
  return sign * approx;
}

float sinh(float x) { return (exp(x) - exp(-x)) / 2; }

float tan(float x) { return sin(x) / cos(x); }

float tanh(float x) {
  float base = exp(x);
  return 1 - 2 / (base * base + 1);
}

// e^x = 2^(x/ln(2)) = 2^(int exponent [n]) + 2^(fractional exponent [x])
float exp(float x) {
  if (x == 0) {
    return 1;
  }
  if (x < 0) {
    return 1 / exp(-x);
  }
  x = x / LN_2;
  int n = (int)x;
  x -= (n + 0.5);
  // powers of ln(2)
  float ln2[13] = {1,
                   LN_2,
                   0.480453013918201,
                   0.333024651988929,
                   0.230835098583083,
                   0.160002697757141,
                   0.110905418832348,
                   0.0768737783724616,
                   0.0532848427378619,
                   0.0369342385103290,
                   0.0256008632895631,
                   0.0177451662090613,
                   0.0123000119263784};
  // Taylor approximate 2^x near 0.5
  // at least power of 2 is fast
  float frac_result = SQRT_2 + (SQRT_2 * x * LN_2) +
                      (pow_int(x, 2) * ln2[2] / SQRT_2) +
                      (pow_int(x, 3) * ln2[3] / (3 * SQRT_2)) +
                      (pow_int(x, 4) * ln2[4] / (12 * SQRT_2)) +
                      (pow_int(x, 5) * ln2[5] / (60 * SQRT_2)) +
                      (pow_int(x, 6) * ln2[6] / (360 * SQRT_2)) +
                      (pow_int(x, 7) * ln2[7] / (2520 * SQRT_2)) +
                      (pow_int(x, 8) * ln2[8] / (20160 * SQRT_2)) +
                      (pow_int(x, 9) * ln2[9] / (181440 * SQRT_2)) +
                      (pow_int(x, 10) * ln2[10] / (1814400 * SQRT_2)) +
                      (pow_int(x, 11) * ln2[11] / (19958400 * SQRT_2)) +
                      (pow_int(x, 12) * ln2[12] / (239500800 * SQRT_2));
  return (1 << n) * frac_result;
}

float frexp(float x, int *exponent) {
  if (x < 0) {
    return -frexp(-x, exponent);
  }
  int n = 1;
  float a = x;
  while (a >= 1.0) {
    a = x / (1 << n++);
  }
  *exponent = n - 1;
  return a;
}

float ldexp(float x, int exponent) { return x * (1 << exponent); }

float ln(float x) {
  if (x < 0) {
    return NAN;
  }
  if (x == 0) {
    return NEG_INF;
  }
  if (x < 1) {
    return -ln(1 / x);
  }
  // factor out a power of 2
  // such that ln(a * 2^n) = ln(a) + n * ln(2)
  // https://math.stackexchange.com/a/3383716
  int n = 1;
  float a = x;
  while (a > 3.0 / 4.0) {
    a = x / (1 << n++);
  }
  a *= 2;
  n -= 2;
  // expand at (1 + a) / 2
  // 1/x = \sum_{k=0}^{\infty} (-1)^k * (2/(a+1))^{k+1} * (x-(1+a)/(2))^k
  // \ln(a) = \int_{1}^{a} 1/x dx = \sum_{k=0}^{\infty}
  // (-1)^k * (2/(a+1))^{k+1} * \int_{1}^{a}(x-(1+a)/2)^k dx
  float base = (a - 1) / (a + 1);
  float ln_a = (2 * base) + (2 * pow_int(base, 3) / 3) +
               (2 * pow_int(base, 5) / 5) + (2 * pow_int(base, 7) / 7) +
               (2 * pow_int(base, 9) / 9) + (2 * pow_int(base, 11) / 11) +
               (2 * pow_int(base, 13) / 13) + (2 * pow_int(base, 15) / 15) +
               (2 * pow_int(base, 17) / 17) + (2 * pow_int(base, 19) / 19);
  return ln_a + n * LN_2;
}

float log(float x) { return ln(x); }

float log10(float x) { return log_any(10, x); }

float log_any(float x, float y) { return ln(y) / ln(x); }

float modf(float x, float *integer) {
  *integer = (int)x;
  return x - (int)x;
}

float pow_int(float x, int n) {
  float a = 1;
  if (n < 0) {
    x = 1 / x;
    n = -n;
  }

  while (n > 0) {
    if (n & 1) {
      a = a * x;
    }
    x = x * x;
    n = n / 2;
  }

  return a;
}

// a^x = e^ln(a)^x = e^(xln(a))
float pow(float x, float n) {
  if (n == 0) {
    return 1;
  }
  return exp(n * ln(x));
}

// Newton's method
// x_n+1 = x_n - f(x_n) / f'(x_n)
// let f(x) = x^2 - S = 0
// x_n+1 = x_n - (x_n^2 - S) / (2 * x_n)
//       = 1/2 * (x_n + S/x_n)
// intuition is that if estimate e is low,
// S / e is high & correct is in between
float sqrt(float x) {
  // keep floating point precision
  float s = 1;
  while (x > 100) {
    x /= 100;
    s *= 10;
  }
  int n = 15;
  float res = x / 2;
  while (n--) {
    res = (res + x / res) / 2;
  }
  return s * res;
}

float ceil(float x) {
  if (x == (int)x) {
    return x;
  }
  return x > 0 ? (int)x + 1 : (int)x;
}

float fabs(float x) { return (x > 0 ? x : -x); }

float floor(float x) {
  if (x == (int)x) {
    return x;
  }
  return x > 0 ? (int)x : (int)x - 1;
}

float fmod(float x, float y) { return x - (int)(x / y) * y; }