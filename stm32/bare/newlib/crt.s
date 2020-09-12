.cpu cortex-m3
.thumb


//.global __aeabi_uidivmod
// end of 20K RAM
.word 0x20005000
.word _reset
.thumb_func
_reset:
	b skip

	// copy initialised data from .sidata (flash) to .data (ram)	
	ldr r0, =sdata
	//ldr r0, [r0]
	ldr r1, =sidata
	//ldr r1, [r1]
	ldr r2, =edata
	//ldr r2, [r2]
	sub r2, r2, r0
	bl memcpy_usr
skip:
	bl init_mem
	bl main
	bl .
//hang:
//	b hang


/*
.thumb_func:
__aeabi_uidivmod:
	push {lr}
	udiv r4, r0, r1
	mul r5, r4, r1
	sub r5, r0, r5

	bl putU
	pop {pc}
 
	push {r0,r1}
	bl _div
	mov r4, r0
	pop {r0, r1}
	bl _mod
	mov r1, r0
	mov r0, r4
	
	push {r0, r1}
	mov r0, #85
	bl putc2
	pop {r0, r1}

	pop { pc}
	//stmdb sp!, {lr}
	//ldmia sp!, {r0-r12, pc}

*/

.data 
.align 4
sdata: .word _sdata
sidata: .word _sidata
edata: .word _edata

