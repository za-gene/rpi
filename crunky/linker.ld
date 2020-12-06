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

