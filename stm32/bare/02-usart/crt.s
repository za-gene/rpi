.cpu cortex-m3
.thumb

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
	//bl memcpy_usr
skip:
	bl init_mem
	bl main
	b .

.data 
.align 4
sdata: .word _sdata
sidata: .word _sidata
edata: .word _edata
