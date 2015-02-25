.globl system_memory_barrier
.globl system_memory_read_barrier               
.globl system_memory_write_barrier       
.globl system_enable_interrupts
.globl system_disable_interrupts

@ Unimplemented for now         
system_memory_barrier:  
    bx lr
        
system_memory_write_barrier:    
    bx lr
        
system_memory_read_barrier:
    bx lr        

system_enable_interrupts:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

system_disable_interrupts:
    mrs r0,cpsr
    orr r0,r0,#0x80
    msr cpsr_c,r0
    bx lr
