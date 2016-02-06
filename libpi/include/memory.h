#ifndef MEMORY_H
#define MEMORY_H

extern unsigned GET32 ( unsigned addr );
extern void PUT32 ( unsigned addr, unsigned word );

extern unsigned GET16 ( unsigned addr );
extern void PUT16 ( unsigned addr, unsigned word );

extern unsigned GET8 ( unsigned addr );
extern void PUT8 ( unsigned addr, unsigned word );

extern unsigned GETPC ( void );

extern void memory_read_barrier( void );
extern void memory_write_barrier( void );

extern void memory_cache_enable( void );
extern void memory_cache_disable( void );

/*
 * Data memory barrier
 * No memory access after the DMB can run until all memory accesses before it
 * have completed
 */
#define dmb() asm volatile \
        ("mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0) )


/*
 * Data synchronisation barrier
 * No instruction after the DSB can run until all instructions before it have
 * completed
 */
#define dsb() asm volatile \
        ("mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0) )


/*
 * Clean and invalidate entire cache
 * Flush pending writes to main memory
 * Remove all data in data cache
 */
#define flushcache() asm volatile \
        ("mcr p15, #0, %[zero], c7, c14, #0" : : [zero] "r" (0) )

#endif
