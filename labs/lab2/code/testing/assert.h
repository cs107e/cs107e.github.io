#ifndef ASSERT_H
#define ASSERT_H

// abort runs an infinite loop that flashes the red power LED on the
// Pi board.
extern void abort(void);

// You call assert on an expression that you expect to be true. If x
// actually evaluates to false, then assert calls abort, which stops
// your program and flashes the red light of doom.
#define assert(x) if(!(x)) abort()

#endif
