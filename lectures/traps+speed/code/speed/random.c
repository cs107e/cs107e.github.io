static unsigned short lfsr = 0xACE1u;
static unsigned bit;

unsigned short rand() {
         bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
        lfsr =  (lfsr >> 1) | (bit << 15);
        return lfsr;
}
