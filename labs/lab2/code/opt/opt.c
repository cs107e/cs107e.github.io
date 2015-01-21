/*
 * A set of simple C snippets that illustrate different optimizations 
 * compilers can do.
 * You should disassemble and work out the mapping from C code to asm.
 */

/**********************************************************************/
int unroll(int *p) {
	int i;
	for(i = 0; i < 3; i++)
		p[i] = 0;
}

/**********************************************************************/
int crazy_loop(char *p, int n) {
	int i;
	for(i = 0; i < n; i+=4) { 
		p[i+0] = 1;
		p[i+1] = 1;
		p[i+2] = 1;
		p[i+3] = 1;
	}
}



/**********************************************************************
 *
 * Switch statements can lead of very different code generation strategies 
 * based on the number and value of case labels.    Here's a set.  
 * What's going on?
 */

// just for reference (but: what happened to the if-statement??)
int if_else(int x) {
 	if(x == 0)
		return 0;
	else
		return 1;
}

// different C code, but...
int two_way_pick(int x) {
	switch(x) {
	case 0:  return 0;
 	default: return 1;
	}
}

// why different from two_way_pick?
int two_way_pick_01(int x) {
	switch(x) {
	case 0:  return 0;
 	case 1: return 1;
	}
}


// now it's getting more interesting.
int n_way_dense(int x) {
	switch(x) {
	case 0:  return 0;
	case 1:  return 1;
	case 2:  return 2;
	case 3:  return 3;
	}
}

// why does the code change the way it does?
int n_way_sparse_small(int x) {
	switch(x) {
	case 0:  return 0;
	case 11:  return 1;
	case 22:  return 2;
	case 33:  return 3;
	}
}

// bam.  crazy talk.
int n_way_sparse_larger(int x) {
	switch(x) {
	case 0:  return 0;
	case 111:  return 1;
	case 222:  return 2;
	case 333:  return 3;
	}
}
