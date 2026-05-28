
void set(int arr[], unsigned long index, int val) {
   arr[index] = val;
}

void eitheror(int arr[], int val) {
    if (val % 2 == 0) {
        arr[8] = val;
    } else {
        arr[8] = 0;
    }
}

int count_bits(unsigned int val) {
    int count = 0;
    while (val != 0) {
        if (val & 1) count++;
        val >>= 1;
    }
    return count;
}