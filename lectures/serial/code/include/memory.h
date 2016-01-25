#ifndef MEMORY_DEFS
#define MEMORY_DEFS

extern unsigned GET32 ( unsigned addr );
extern void PUT32 ( unsigned addr, unsigned word );

extern unsigned GET16 ( unsigned addr );
extern void PUT16 ( unsigned addr, unsigned word );

extern unsigned GET8 ( unsigned addr );
extern void PUT8 ( unsigned addr, unsigned word );

extern unsigned GETPC ( void );

#endif
