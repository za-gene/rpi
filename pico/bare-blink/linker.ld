/* source:
 * https://github.com/rp-rs/pico-blink-rs/blob/develop/memory.x
 */

MEMORY
{
	/* NOTE 1 K = 1 KiBi = 1024 bytes */
	/* To suit Raspberry Pi RP2040 SoC */
	BOOT_LOADER : ORIGIN = 0x10000000, LENGTH = 0x100
	FLASH : ORIGIN = 0x10000100, LENGTH = 2048K - 0x100
	RAM : ORIGIN = 0x20000000, LENGTH = 264K
}

SECTIONS {

	.boot_loader ORIGIN(BOOT_LOADER) :
	{
		__boot2_start__ = .;
		KEEP(*(.boot_loader*));
		__boot2_end__ = .;
	} > BOOT_LOADER
	ASSERT(__boot2_end__ - __boot2_start__ == 256, 
		"ERROR: Pico second stage bootloader must be 256 bytes in size")

	.text ORIGIN(FLASH):
	{
		. = ALIGN(4);
		*(.text*)
		. = ALIGN(4);
	} > FLASH

	.userstack :
	{
		. = ALIGN(4);
		. = . + 0x0400; /* minimum stack size */
		. = ALIGN(4);
		__StackTop = .;
	} > RAM



}


