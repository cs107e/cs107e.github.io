extern int __bss_start__;
extern int __bss_end__;
extern int _table;
extern int _table_end;

void notmain();
 
#define RPI_VECTOR_START 0x0

void _cstart() {
  /* Zero out BSS */
  int* bss = &__bss_start__;
  int* bss_end = &__bss_end__;
  while( bss < bss_end )
    *bss++ = 0;
  
  /* Copy in interrupt vector table and FIQ handler */
  /* _table and _table_end are symbols defined in the interrupt
     assembly file, at the beginning and end of the table and
     its embedded constants.*/
  int* vector = (int*)RPI_VECTOR_START;
  int* table = &_table; 
  int* table_end = &_table_end;
  while (table < table_end) {
    *vector = *table;
    vector++;
    table++;
  }

  notmain(); 
}
