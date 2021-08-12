#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include <math.h>


#include "crc8.h"
#include "fat32.h"
#include "mcp4921-dma.h"
#include "pi.h"
#include "ssd1306.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()


//#define ADDRESS (XIP_BASE+ FLASH_TARGET_OFFSET)
#define FLASH_TARGET_OFFSET (256 * 1024)
#define ADDRESS (XIP_BASE+ FLASH_TARGET_OFFSET)
static_assert(FLASH_PAGE_SIZE == 256);

uint32_t flash_from_sdcard(const char dos83_filename[12])
{
	fat32_init();

	File file(dos83_filename);
	unsigned char crc = 0;
	puts("Reading file");


	uint8_t page[4096]; // I say page, but I mean sector
	uint32_t offset = FLASH_TARGET_OFFSET;
	int nread = 0;

	unsigned char crc_file = 0;
	while(nread < file.size()) {
		// read a page from file
		int num_bytes_in_page_read = 0;
		for(int i = 0; i< 4096/512; i++) {
			num_bytes_in_page_read += file.read(page + i*512);
		}
		if(num_bytes_in_page_read<4096) 
			memset(page + num_bytes_in_page_read, 0, 4096-num_bytes_in_page_read); // kindly fill the remainder with 0
		crc_file = crc8_dallas_chunk(crc_file, page, num_bytes_in_page_read);
		nread += num_bytes_in_page_read;

		// write page to flash 
		uint32_t ints = save_and_disable_interrupts();
		flash_range_erase(offset, 4096);
		flash_range_program(offset, page, 4096);
		restore_interrupts(ints);
		offset += 4096;
	}
	puts("File was written. Contents are:");
	//uart_write_blocking(uart0, (const uint8_t *) ADDRESS, file.size());


	printf("\nCRC on file: %d\n", crc_file);
	printf("CRC on flash: %d\n", crc8_dallas((const unsigned char*) ADDRESS, file.size()));

	return file.size();

}

#define ALARM 0
#define DELAY (1'000'000/ 16'000)
volatile uint32_t size = 0, idx = 0;

static void alarm_0_irq() 
{
	pi_alarm_rearm(ALARM, DELAY);
	if(idx >= size) idx = 0;
	volatile uint8_t vol8 = *(uint8_t*)(ADDRESS + idx);
	volatile uint16_t vol16 = vol8; 
	vol16 <<= 4;
	mcp4921_dma_put(vol16);
	idx++;
}

void song_init()
{	
	mcp4921_dma_init();
	pi_alarm_init(ALARM, alarm_0_irq, DELAY);
}

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	printf("ADDRESS = %d\n", ADDRESS);
	printf("XIP_BASE = %d\n", XIP_BASE);
	printf("FLASH_TARGET_OFFSET = %d\n", FLASH_TARGET_OFFSET);
	printf("FLASH_PAGE_SIZE = %d\n", FLASH_PAGE_SIZE); // 256
	printf("FLASH_SECTOR_SIZE = %d\n", FLASH_SECTOR_SIZE); // 4096
	//printf(" = %d\n", );


	char dosname[12];
	canfile(dosname, "song.raw");
	size = flash_from_sdcard(dosname);

	song_init();

	while(1);

	//init_display(64, 4);

}

