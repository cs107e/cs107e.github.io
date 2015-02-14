#include "system.h"

void memory_read_barrier() {
  // DSB: data synchronization barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
}

void memory_write_barrier() {
  // DSB: data synchronization barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
}

void memory_barrier() {
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
