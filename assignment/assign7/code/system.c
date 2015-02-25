#include "system.h"

void system_enable_branch_prediction() {
  unsigned int r;
  asm volatile("MRC p15, 0, %0, c1, c0, 0" : "=r" (r));
  r |= 0x0800;
  asm volatile("MCR p15, 0, %0, c1, c0, 0" :: "r" (r));

  // Invalidate caches, including branch cache
  r = 0;
  asm volatile("MCR p15, 0, %0, c7, c7, 0" :: "r" (r));
}

void system_enable_caches() {
  unsigned int r;
  asm volatile("MRC p15, 0, %0, c1, c0, 0" : "=r" (r));
  r |= 0x1004;
  asm volatile("MCR p15, 0, %0, c1, c0, 0" :: "r" (r));

  // Invalidate both caches and clean entire data cache
  r = 0;
  asm volatile("MCR p15, 0, %0, c7, c7, 0" :: "r" (r));
  asm volatile("MCR p15, 0, %0, c7, c10, 0" :: "r" (r));

}

void system_enable_interrupts() {
  asm("mrs r0, cpsr");
  asm("bic r0, r0, #0x80"); 
  asm("msr cpsr_c, r0");
}

/* TO DO: should return a bool specifying whether interrupts
   were disabled when called, so nested disables can work. */
void system_disable_interrupts() {
  asm("mrs r0, cpsr");
  asm("orr r0, r0, #0x80");
  asm("msr cpsr_c, r0");
}

void system_memory_read_barrier() {
  // DSB: data synchronization barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
}

void system_memory_write_barrier() {
  // DSB: data synchronization barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
}

void system_memory_full_barrier() {
  // Borrowed from "Circle" C++ interrupt code
  // Clean data cache
  asm volatile ("mcr p15, 0, %0, c7, c10, 0" : : "r" (0) : "memory");
  // Data sync barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
  
  // Invalidate instruction cache
  asm volatile ("mcr p15, 0, %0, c7, c5,  0" : : "r" (0) : "memory");
  // Flush branch target cache
  asm volatile ("mcr p15, 0, %0, c7, c5,  6" : : "r" (0) : "memory");

  // Data sync barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");

  // Flush prefetch buffer
  asm volatile ("mcr p15, 0, %0, c7, c5,  4" : : "r" (0) : "memory");
  // Data memory barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory");
}
