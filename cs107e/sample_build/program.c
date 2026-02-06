

void big_stack_function(void) {
    char buffer[4096];  // Large allocation
    buffer[0] = 1;
    // Prevent optimization
    asm volatile("" : : "r"(buffer) : "memory");
}

int factorial(int n) {
    return n * factorial(n-1);
}

int main(void)
{
    return factorial(10);
}

//void *__stack_limit = (void*)0x80000000;  // Example address
