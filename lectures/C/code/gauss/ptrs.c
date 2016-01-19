void basics()
{
	int m, n, *p, *q;

	p = &n;
	*p = n;      // same as prev line?

	q = p; 
	*q = *p;     // same as prev line?

	p = &m, q = &n;
	*p = *q; 
	m = n;       // same as prev line?
}

void ptr_array()
{
	int n, arr[4], *p;

	p = arr;
	p = &arr[0]; // same as prev line?

	*p = 3;
	p[0] = 3;    // same as prev line?

	n = *(arr + 1); 
	n = arr[1];  // same as prev line?
}

void gpio()
{
	char *mem = (char *)0;
	int *gpio = (int *)0x20200000;

	// which of these are same?

	mem[0x2020008] = 1;
	*(int *)(mem + 0x2020008) = 1;

	*(gpio + 0x8) = 1;
	gpio[2] = 1;
}