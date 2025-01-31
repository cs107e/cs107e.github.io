#include "printf.h"
#include "timer.h"
#include "uart.h"

void echo(void) {
    int ch;

    printf("Type characters (end with q): ");
    do {
        ch = uart_getchar(); // read char laptop TX -> mango RX
        uart_putchar(ch);        // write char mango TX -> laptop RX
    } while (ch != 'q');
}

void arrays(void) {
  // Note '\0' at the end!
  char arr[] = {'c','s','1','0','7','e','\0'};
  printf("sizeof arr: %ld\n", sizeof arr);

  // short cut
  // char arr[] = "cs107e";

  char ch = arr[1]; // ok!
  ch = *(arr + 1); // also ok!
  printf("ch: %c\n", ch);
  printf("ch in decimal: %d\n", ch);
  printf("ch in hex: 0x%x\n", ch);

  char *ptr = arr; // ok!
  printf("sizeof ptr: %ld\n", sizeof ptr);
  // arr = ptr;       // not ok, why?
  // arr = arr + 1;   // also not okay!
  // ptr = ptr + 1;   // this is okay!
  printf("arr: %s\n", arr);
  printf("address arr: %p\n", arr);
  printf("address &arr: %p\n", &arr);
  printf("address &arr[0]: %p\n", &arr[0]);
  printf("address ptr: %p\n", ptr);
  printf("address &ptr: %p\n", &ptr);

  printf("ptr as string: %s\n", ptr);

  char *ptr_str = "cs107e";
  printf("ptr_str: %s\n", ptr_str);
  ch = ptr_str[1];  // ok?
  ptr_str[1] = 'd'; // ok?
  printf("ptr_str: %s\n", ptr_str);

  // whackiness follows
  char *literal_str = "hello!";
  literal_str[0] = 'X'; // disallowed by C! The compiler does not complain...
  printf("literal_str: %s\n", literal_str);
  printf("the string \"hello!\": %s\n", "hello!");
}

void main(void) {
  uart_init();

  /*
  echo();

  for (int num = 100; num > 0; num--) {
    printf("\n%d bottles of beer on the wall, %d bottles of beer.\n",
        num, num);
    printf("Take one down and pass it around, %d bottles of beer on the wall.\n",
        num - 1);
    timer_delay(1);
  }
  */
  arrays();
}
