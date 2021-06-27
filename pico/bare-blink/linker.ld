/* source:
 * https://github.com/rp-rs/pico-blink-rs/blob/develop/memory.x
 */

/*
ENTRY(reset_handler)
*/

MEMORY
{
	/* NOTE 1 K = 1 KiBi = 1024 bytes */
	/* To suit Raspberry Pi RP2040 SoC */
	BOOT2 : ORIGIN = 0x10000000, LENGTH = 0x100 
	FLASH : ORIGIN = 0x10000100, LENGTH = 2048K  - 0x100
	/* FLASH : ORIGIN = 0x10000000, LENGTH = 2048K */

	RAM : ORIGIN = 0x20000000, LENGTH = 264K
}

SECTIONS {

	
	.boot2 :
	{
		__boot2_start__ = .;
		*(.boot2*);
		__boot2_end__ = .;
	} >BOOT2
	ASSERT(__boot2_end__ - __boot2_start__ == 256, 
		"ERROR: Pico second stage bootloader must be 256 bytes in size")

	.text :
	{
		/*
		__boot2_start__ = .;
		*(.boot2*);
		__boot2_end__ = .;
		*/

		*(.vectors*)
		. = ALIGN(4); 
		*(.text*)
		. = ALIGN(4);
	} >FLASH
	/*
	ASSERT(__boot2_end__ - __boot2_start__ == 256, 
		"ERROR: Pico second stage bootloader must be 256 bytes in size")
		*/
		

	.userstack :
	{
		. = ALIGN(4);
		. = . + 0x0400; /* minimum stack size */
		. = ALIGN(4);
		__StackTop = ORIGIN(RAM) + LENGTH(RAM);
	} > RAM



}


