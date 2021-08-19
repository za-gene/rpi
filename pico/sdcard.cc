/* File released into the Public Domain. Fill yer boots ;)
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
//#include "tusb.h" // if you want to use tud_cdc_connected()

#include "pace.h"
//#include "../../1306/pico-sdk/oled.h"

//                                SHIELD
#define spi		spi1
#define	PIN_SCK		10	// D5
#define	PIN_MOSI	11	// D7
#define PIN_MISO 	12	// D6
#define	PIN_CS 		15	// D8

//#define SPK 7
//#define SPK 19

#define CMD_TIMEOUT 200 // number of tries before getting bored
#define R1_IDLE_STATE (1<<0)
#define R1_ILLEGAL_COMMAND (1<<2)

#define SDTOUT -2 // timeout waiting for card to be ready
#define SDROUT -3 // timeout waiting for card to respond to command
#define SDCMD0 -4 // failed CMD0 (card couldn't be set to idle state)
#define SDCMD8 -5 // failed CMD8 (identify card version (only version 2 is handled))
#define SDFV2  -6 // failed to initialise card as version 2 type card
#define SDCMD16 -7 // CMD16 failed to set card block size to 512
#define SDBLOCK -8 // bad read of block
#define SDCDV   -9 // implementation limitation on cdv. 

void cs_low() {	gpio_put(PIN_CS, 0); }

void cs_high() { gpio_put(PIN_CS, 1); }

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

int cdv = 512; // card is byte addressing, set to 1 if addresses are per block


int wait_for_ready()
{
	u8 dst;
	uint32_t start = time_us_32();
	while(time_us_32() - start < 300'000) {
		spi_read_blocking(spi, 0xFF, &dst, 1);
		if(dst == 0xFF) return 0;
	}
	return SDTOUT;

}

class Trans {
	public:
		Trans();
		~Trans();
};

Trans::Trans()
{
	cs_low();
}

Trans::~Trans()
{
	cs_high();
	uint8_t b = 0xFF;
	spi_write_blocking(spi, &b, 1); // just spin our wheels so that the card can complete its operation
}


u8 crc_table[256];

void init_crc_table()
{
	const u8 crc_poly = 0x89;
	for(int i = 0; i< 256; i++) {
		if(i & 0x80)
			crc_table[i] = i ^ crc_poly;
		else
			crc_table[i] = i;

		for(int j = 1; j<8; j++) {
			crc_table[i] <<= 1;
			if(crc_table[i] & 0x80)
				crc_table[i] ^= crc_poly;
		}
	}
}

u8 calculate_crc(u8* buf)
{
	static bool table_initialised = false;
	if(!table_initialised) {
		init_crc_table();
		table_initialised = true;
	}

	u8 crc = 0;
	for(int i=0; i<5; i++)
		crc = crc_table[(crc<<1)^ buf[i]];
	return (crc<<1) | 1;
}

u8 encode_cmd(int cmd, int arg, int crc, u8 buf[6])
{
	buf[0] = 0x40 | cmd;
	buf[1] = (arg >> 24) & 0xFF;
	buf[2] = (arg >> 16) & 0xFF;
	buf[3] = (arg >> 8) & 0xFF;
	buf[4] = (arg >> 0) & 0xFF;

	if(crc==0) crc = calculate_crc(buf);

	buf[5] = crc;
	return crc;
}

/* return the (non-negative) response from an issued command
 * SDROUT (a neg number) if timed out
 */

int wait_for_response()
{
	uint8_t resp;
	for(int i = 0; i< CMD_TIMEOUT; i++) {
		spi_read_blocking(spi, 0xFF, &resp, 1);
		if(!(resp & 0x80)) return resp;
	}
	return SDROUT;

}

int call_cmd(int cmd, int arg, int crc)
{
	uint8_t buf[6];
	encode_cmd(cmd, arg, crc, buf);
	spi_write_blocking(spi, buf, sizeof(buf));
	return wait_for_response();
}

int CMD_T1(int cmd, int arg, int crc)
{
	Trans t;
	int status;
	//if(wait_for_ready()) return SDTOUT;
	wait_for_ready();
	status = call_cmd(cmd, arg, crc);
	if(status <0) return SDROUT;

	return status;
}

// NB the len is only ever of size 4
int CMD_T2 (int cmd, int arg, int crc, u8* output, int len)
{
	Trans t;
	//if(wait_for_ready()) return SDTOUT;
	int status = wait_for_ready();
	if(status) return SDTOUT;

	status = call_cmd(cmd, arg, crc);
	if(status <0) return SDROUT;

	spi_read_blocking(spi, 0xFF, output, len);

	return status;
}

int CMD_T3 (int cmd, int arg, int crc, u8* output, int len)
{
	Trans t;
	//if(wait_for_ready()) return SDTOUT;
	int status = wait_for_ready();
	if(status) return SDTOUT;

	status = call_cmd(cmd, arg, crc);
	if(status<0) return SDROUT;

	u8 buf[6];
	buf[0] = 0xFF;
	while(buf[0] != 0xFE)
		spi_read_blocking(spi, 0xFF, buf, 1);

	// read the actual data
	spi_read_blocking(spi, 0xFF, output, len);

	// read crc
	spi_read_blocking(spi, 0xFF, buf, 2); 

	return status;
}

int init_card_v2()
{
	u8 ocr[4];
	for(int i=0; i< CMD_TIMEOUT; i++) {
		printf("init_card_v2 try %d\n", i);
		sleep_ms(50);
		CMD_T2(58, 0, 0xFD, ocr, sizeof(ocr));
		CMD_T1(55, 0, 0x65);
		constexpr int arg = 0x40000000;
		static_assert((arg>0) && (sizeof(int)>=4));
		if(CMD_T1(41, arg, 0x77) == 0) {
			CMD_T2(58, 0, 0xFD, ocr, sizeof(ocr));
			if((ocr[0] & 0x40) != 0) cdv = 1;
			return 0;
		}
	}
	return SDFV2;
}

int init_card()
{
	sleep_ms(1); // give card time to power up

	//init_crc_table();

	// standard spi stuff
	int spi_speed = 1'200'000;
	spi_speed = 600'000; // works
	//spi_speed = 250'000;
	spi_speed = 100'000; // slow to get initialisation
	spi_init(spi, spi_speed);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_set_dir(PIN_CS, GPIO_OUT);


	// set card into spi mode: clock card at least 80 cycles with cs high
	printf("Set card to SPI mode ... ");
	cs_high();
	for(int i = 0; i < 10; i++) {
		uint8_t b = 0xFF;
		spi_write_blocking(spi, &b, 1);		
	}
	if(gpio_get(PIN_CS))
		printf("CS high. Bad juju.\n");
	else
		printf("CS low. Good.\n");

	// CMD0 go idle
	int status;
	for(int i = 0; i < 10; i++) { // repeated attempts
		printf("go idle attempt %d ... ", i);
		status = CMD_T1(0, 0, 0x95);
		printf("status=%d (-3=response timeout)\n", status);
		if(status == R1_IDLE_STATE) {
			//ssd1306_print(" IDLE ");
			break;
		}
		sleep_ms(200); // didn't help
	}
	if(status == R1_IDLE_STATE) {
		printf("CMD0 succeeded\n");
	} else {
		printf("CMD0 failed. Aborting initialisation\n");
		return SDCMD0;
	}

	// CMD8: card version
	// We only consider version 2 cards at this point
	u8 cmd8_response[4];
	status = CMD_T2(8, 0x01aa, 0x87, cmd8_response, sizeof(cmd8_response));
	printf("\nCMD8 card status %d\n", status);
	if(status != R1_IDLE_STATE) 
		return SDCMD8;

	// we are assuming version 2 cards
	printf("Attempting init_card_v2()\n");	
	status = init_card_v2();
	if(status) return status;

	// skip CMD9 that adafruit uses


	// CMD16 set block length to 512 bytes
	if(CMD_T1(16, 512, 0x15) != 0) return SDCMD16;
	printf("CMD16 set block size to 512 successfully\n");

	spi_speed = 1'000'000; // go for it! Known working: 1MHz, 2MHz, 4MHz, 8MHz, 16MHz, 24MHz
	// Corruptions:  32MHz
	spi_speed = 24'000'000;
	spi_set_baudrate(spi, spi_speed);

	return 0;
}

int block_cmd(int cmd, int blocknum, u8 block[512])
{
	//printf("block_cmd: cdv = %d\n", cdv);
	if(cdv != 1) return SDCDV;

	return CMD_T3(cmd, blocknum, 0, block, 512);

	return 0;
}

int readablock (int blocknum, u8 block[512])
{
	if(block_cmd(17, blocknum, block) != 0)
		return SDBLOCK;
	return 0;
}

void dump_block(u8 block[512])
{
	for(int row = 0; row < 512/16; row++) {
		printf("%04x  ", row*16);
		for(int col = 0; col<16; col++) {
			printf("%02x ", block[row * 16 + col]);
			if(col == 7) printf(" ");
		}
		printf(" ");

		// asci version of row, where possible
		for(int col = 0; col<16; col++) {
			char c = block[row * 16 + col];
			if((c<32) || (c>126)) c = '.';
			printf("%c", c);
			//if(col == 7) printf(" ");
		}
		printf("\n");
	}
}

// let's see that se can calculate CRC correctly
// looks pretty good based on the couple of examples
void test_crc()
{
	printf("\ntest_crc begin\n");
	u8 crc, buf[6];

	crc = encode_cmd(58, 0, 0, buf);
	printf("Expected %d, got %d\n", (int) 0xFD, (int) crc);

	crc = encode_cmd(8, 0x01aa, 0, buf);
	printf("Expected %d, got %d\n", (int) 0x87, (int) crc);

	printf("test_crc end\n");
}

void sdcard_init(void)
{
	init_card();
}

void sdcard_deinit(void)
{
	// nothing to do, really
}

