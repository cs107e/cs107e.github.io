/* 
 * This is just a simple recursive function. 
 * Recompile with -O2 and run it.  
 * What happened?
 */

int f(int x) {
  if (x == 0) {
    return 0;
  } else {
    return x + f(x-1);
  }
}

int notmain(void) {
  return f(30000);
}
