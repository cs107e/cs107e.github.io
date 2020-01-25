#include "tricky.h"

// uninitialized global and static variables
int i;
static int j;

// initialized global and static variables
int k = 1;
int l = 0;
static int m = 2;

// initialized global and static const variables
const int n = 3;
static const int o = 4;

void tricky(int x)
{
   int r = k;
   i = k + m + n + r;
   j = i + x;

   int *rptr = &r;
   int d = *rptr;

   k = i;
   l = j;
   k = k + d;
}
