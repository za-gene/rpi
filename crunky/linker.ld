/* bss sections mentioned here:
 * https://stackoverflow.com/q/43922511/4037492
 */

SECTIONS
{ 
        .text.boot 0x8000 : { *(.text.boot) }
        .text : { *(.text) }
        .rodata : { *(.rodata) }
        .data : { *(.data) }
        . = ALIGN(0x8);
	
	.bss : { 
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss*);
		*(.bss*);
		*(COMMON);
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} 

/* C++ stuff https://github.com/arobenko/embxx_on_rpi/blob/master/src/raspberrypi.ld
mcarter added 2021-02-06
*/
    .init.array :
    {
        __init_array_start = .;
        *(.init_array)
        *(.init_array.*)
        __init_array_end = .;
    }

/* for C++
        https://stackoverflow.com/questions/53751272/enabling-c-exceptions-on-arm-bare-metal-bootloader
mcarter added 2021-02-06
*/
    .ARM.exidx :            /* Required for unwinding the stack? */
    {
        __exidx_start = .;
        * (.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end   = .;
    } 

	.crunky_heap : {
		. = ALIGN(4);
		_sheap = .;
		__heap_start__ =  _sheap;
		. = . + 100000000; /* 100MB heap */
		. = ALIGN(4);
		_eheap = .;
		__heap_end__ = _eheap;
	}



}

