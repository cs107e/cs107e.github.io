#ifndef __DEMAND_H__
#define __DEMAND_H__

void FFatal(const char *file, int line, const char *fmt, ...);
#define fatal_err(msg...) FFatal(__FILE__, __LINE__, ##msg)

/* Compile-time assertion used in function. */
#define AssertNow(x) switch(1) { case (x): case 0: }

/* Compile time assertion used at global scope; need expansion. */
#define _assert_now2(y) _ ##  y
#define _assert_now1(y) _assert_now2(y)
#define AssertNowF(x)                                                   \
  static inline void _assert_now1(__LINE__)()                           \
        { AssertNow(x); (void)_assert_now1(__LINE__);  /* shut up -Wall. */}

#endif /*__DEMAND_H__ */
