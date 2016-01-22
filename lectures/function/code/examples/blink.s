.global _start

_start:

	ldr r0, =0x20200008
	mov r1, #1         // configure GPIO 20 for output
	str r1, [r0]
	mov r1, #(1<<20)

loop: 

	ldr r0, =0x2020001C	// set GPIO 20 high
	str r1, [r0] 		
	mov r2, #0x3F0000	// delay
	subs r2, #1
	bne .-4
	ldr r0, =0x20200028	// set GPIO 20 low
	str r1, [r0] 		
	mov r2, #0x3F0000	// delay
	subs r2, #1
	bne .-4

	b loop
