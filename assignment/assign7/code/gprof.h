/*
 * Implement these functions.
 */



typedef void (*gprof_fp_t)(void *data, unsigned pc, unsigned count);

// number of gprof interrupts.
extern unsigned gprof_counter;
// number of interrupts that were outside the text segment.
extern unsigned gprof_overrun;

// set gprof to fire an interrupt every ticks clock cyles.
void gprof_init(unsigned ticks);

// print all the non-zero counts.
void gprof_dump(void);

// call fp(data, pc, count) for each entry in the table.
void gprof_callback(gprof_fp_t fp, void *data);
