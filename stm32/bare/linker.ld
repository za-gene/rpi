/*
 * Minimal linker script for an STM32F103C8 / STM32F103CB.
 * These days, most "64KB" C8 chips actually have 128KB of Flash.
 */

/* Enforce emmition of the vector table. */
EXTERN(vector_table)

/* Label for the program's entry point */
ENTRY(Reset_Handler)

/* Set minimum size for stack and dynamic memory. */
/* (The linker will generate an error if there is
 * less than this much RAM leftover.) */
/* (1KB) */
_Min_Heap_Size = 10K;
/* _Min_Leftover_RAM = 0x0400; */
_Min_Stack_Size = 0x0400;

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(RAM) + LENGTH(RAM); /* higher addr of user mode stack */

SECTIONS
{
  /* The vector table goes to the start of flash. */
	/*
  .vectors :
  {
    . = ALIGN(4);
    KEEP (*(.vectors))
    . = ALIGN(4);
  } >FLASH
  */

  /* The 'text' section contains the main program code. */
  .text :
  {
    . = ALIGN(4);
    KEEP(*(.vectors))
    //*(.text)
    *(.text*)
    . = ALIGN(4);
    KEEP (*(.init))
    KEEP (*(.fini))
    KEEP (*(.eh_frame))
    . = ALIGN(4);
  } >FLASH

  /* Sections required by the standard libraries. */
  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH
  .ARM :
  {
    *(.ARM.exidx*)
  } >FLASH

  /* The 'rodata' section contains read-only data,
   * constants, strings, information that won't change. */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* The 'data' section is space set aside in RAM for
   * things like variables, which can change. */
  _sidata = .;
  .data : AT(_sidata)
  {
    . = ALIGN(4);
    /* Mark start/end locations for the 'data' section. */
    _sdata = .;
    *(.data)
    *(.data*)
    _edata = .;
    . = ALIGN(4);
  } >RAM

  /* The 'bss' section is similar to the 'data' section,
   * but its space is initialized to all 0s at the
   * start of the program. */
  .bss :
  {
    . = ALIGN(4);
    /* Also mark the start/end of the BSS section. */
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM
  /* Mark the end of statically-allocated RAM. */
  end = .;
  _end = end;
  __end = end;

/* C++ stuff https://github.com/arobenko/embxx_on_rpi/blob/master/src/raspberrypi.ld
mcarter added 2020-12-18
*/
    .init.array :
    {
        __init_array_start = .;
        *(.init_array)
        *(.init_array.*)
        __init_array_end = .;
    } >RAM

    /* for C++
        https://stackoverflow.com/questions/53751272/enabling-c-exceptions-on-arm-bare-metal-bootloader
mcarter added 2020-12-18
*/
    .ARM.exidx :            /* Required for unwinding the stack? */
    {
        __exidx_start = .;
        * (.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end   = .;
    } >RAM


    /* Space set aside for the application's heap/stack. */
    ._user_heap_stack :
    {
        . = ALIGN(4);
        _heap_begin = .;
        . = . + _Min_Heap_Size;
        . = ALIGN(4);
        _heap_end = .;
    } >RAM

    /* stack. Make sure we have enough */
    ._usrstack :
    {
        . = ALIGN(4);
        _susrstach = .;
        . = . + _Min_Stack_Size ;
        . = ALIGN(4);
        _eusrstack = . ;
    } >RAM
    

PROVIDE( _sheap = _heap_begin);
PROVIDE( _eheap = _heap_end);

}

