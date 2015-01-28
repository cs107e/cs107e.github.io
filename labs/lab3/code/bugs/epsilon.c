/*
 * Compile with -O2.  What is going on?
 * Add a single volatile to make the arm asm match the C code.
 */

static void delay(int *n) {
	int i;

	for(i = 0; i < *n; i++) 
		;
}

int notmain()  {
	int n = 100000;
	delay(&n);
	return 0;
}
