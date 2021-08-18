;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

//#include <basal.h>

#if RPI == 0
//#warning "vectors reports RPI is 0"
#endif

.section .text.boot
.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word IRQ_handler
fiq_handler:        .word hang

reset:
	
#if RPI != 0
    mrs r0,cpsr        				;@ moving to HYPERVISOR mode
    bic r0,r0,#0x1F
    orr r0,r0,#0x13
    msr spsr_cxsf,r0
    add r0,pc,#4
    msr ELR_hyp,r0
    eret
#endif

    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    mov sp,#0x4000

;@ 	The following are used to set the stack for alternative operating modes.
;@  In the present case, they are commented, thus ignored.
	
    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)		
    ;@ mov r0,#0xD3
    ;@ msr cpsr_c,r0
    ;@ mov sp,#0x8000000

    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
    ;@mov r0,#0x53
    ;@msr cpsr_c, r0
    
	bl premain
	bl kernel_main
hang: b hang

.globl put32
put32:
    str r1,[r0]
    bx lr

.globl get32
get32:
    ldr r0,[r0]
    bx lr

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

;@ From Bruce Smith's ARM32 ASM book page 283
.globl disable_irq
disable_irq:
    mrs r0,cpsr
    orr r0,r0,#0x80
    msr cpsr_c,r0
    bx lr
