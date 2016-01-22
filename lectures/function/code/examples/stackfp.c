// This file is compiled -fno-omit-frame-pointer
// Look at generated asm to see how fp is used
// as per-activation anchor

int C(int z)
{
    return z + 9;
}

int B(int y)
{
    return C(y) - 5;
}

int A(int x)
{
	int arr[5];
    return B(arr[4]);
}

int main(void)
{   
    return A(107);
}

