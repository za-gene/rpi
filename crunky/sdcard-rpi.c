/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <basal.h>
#include <stdio.h>
#include <gpio.h>
#include <stdarg.h>
#include <timers.h>
//#include <sd.h>

#define SD_OK                0
#define SD_TIMEOUT          -1
#define SD_ERROR            -2

/* for some bizarre reason, it seems to be necessary turn on
 * debugging, or else reading will fail
 */

static int debug = 1;

void wait_usec(int i)
{
	delay_us(i);
}

static void sd_uart_puts(const char *fmt, ...)
{
	if(debug==0) return;
	va_list va;
        va_start(va, fmt);
        printf(fmt, va);
        va_end(va);
	puts("");
}

static void sd_error_puts(const char *str)
{
	puts(str);
}

/**
 * Display a binary value in hexadecimal
 */
static void sd_uart_hex(unsigned int d) 
{
	if(debug==0) return;
	unsigned int n;
	int c;
	for(c=28;c>=0;c-=4) {
		n=(d>>c)&0xF; // get highest tetrad
		n+=n>9?0x37:0x30; // 0-9 => '0'-'9', 10-15 => 'A'-'F'
		putchar(n);
	}
}

//#define REG(addr) (*(volatile unsigned int*)(PBASE+addr))

#define EMMC_ARG2           ((volatile unsigned int*)(PBASE+0x00300000))
#define EMMC_BLKSIZECNT     ((volatile unsigned int*)(PBASE+0x00300004))
#define EMMC_ARG1           REG(PBASE+0x00300008)
#define EMMC_CMDTM          REG(PBASE+0x0030000C)
#define EMMC_RESP0          REG(PBASE+0x00300010)
#define EMMC_RESP1          REG(PBASE+0x00300014)
#define EMMC_RESP2          REG(PBASE+0x00300018)
#define EMMC_RESP3          REG(PBASE+0x0030001C)
#define EMMC_DATA           ((volatile unsigned int*)(PBASE+0x00300020))
#define EMMC_STATUS         ((volatile unsigned int*)(PBASE+0x00300024))
#define EMMC_CONTROL0       ((volatile unsigned int*)(PBASE+0x00300028))
#define EMMC_CONTROL1       ((volatile unsigned int*)(PBASE+0x0030002C))
#define EMMC_INTERRUPT      ((volatile unsigned int*)(PBASE+0x00300030))
#define EMMC_INT_MASK       ((volatile unsigned int*)(PBASE+0x00300034))
#define EMMC_INT_EN         ((volatile unsigned int*)(PBASE+0x00300038))
#define EMMC_CONTROL2       ((volatile unsigned int*)(PBASE+0x0030003C))
#define EMMC_SLOTISR_VER    ((volatile unsigned int*)(PBASE+0x003000FC))

#define R1_IDLE_STATE 		(1<<0)
#define R1_ILLEGAL_COMMAND	(1<<2)


// command flags
#define CMD_NEED_APP        0x80000000
#define CMD_RSPNS_48        0x00020000
#define CMD_ERRORS_MASK     0xfff9c004
#define CMD_RCA_MASK        0xffff0000

// COMMANDs
#define CMD_GO_IDLE         0x00000000
#define CMD_ALL_SEND_CID    0x02010000
#define CMD_SEND_REL_ADDR   0x03020000
#define CMD_CARD_SELECT     0x07030000
#define CMD_SEND_IF_COND    0x08020000
#define CMD_STOP_TRANS      0x0C030000
#define CMD_READ_SINGLE     0x11220010
#define CMD_READ_MULTI      0x12220032
#define CMD_SET_BLOCKCNT    0x17020000
#define CMD_WRITE_SINGLE    0x18220000
#define CMD_WRITE_MULTI     0x19220022
#define CMD_APP_CMD         0x37000000
#define CMD_SET_BUS_WIDTH   (0x06020000|CMD_NEED_APP)
#define CMD_SEND_OP_COND    (0x29020000|CMD_NEED_APP)
#define CMD_SEND_SCR        (0x33220010|CMD_NEED_APP)

#define  CMDDESC(x) {x, #x}
typedef struct {
	int num;
	char* desc;
} command_t;

static command_t command_table[] = {
	CMDDESC(CMD_GO_IDLE),
	CMDDESC(CMD_ALL_SEND_CID),
	CMDDESC(CMD_SEND_REL_ADDR),
	CMDDESC(CMD_CARD_SELECT),
	CMDDESC(CMD_SEND_IF_COND),
	CMDDESC(CMD_STOP_TRANS),
	CMDDESC(CMD_READ_SINGLE),
	CMDDESC(CMD_READ_MULTI),
	CMDDESC(CMD_SET_BLOCKCNT),
	CMDDESC(CMD_WRITE_SINGLE),
	CMDDESC(CMD_WRITE_MULTI),
	CMDDESC(CMD_APP_CMD),
	CMDDESC(CMD_SET_BUS_WIDTH),
	CMDDESC(CMD_SEND_OP_COND),
	CMDDESC(CMD_SEND_SCR)
};

char unknown_command[] = "UNKNOWN";

char* describe_cmd(int num) 
{
	for(int i = 0; i< sizeof(command_table)/sizeof(command_t); i++) {
		int table_num = command_table[i].num;
		if((table_num == num) || (table_num == (num|CMD_NEED_APP))) 
			return command_table[i].desc;
	}
	return unknown_command;
}
	


// STATUS register settings
#define SR_READ_AVAILABLE   0x00000800
#define SR_WRITE_AVAILABLE  0x00000400
#define SR_DAT_INHIBIT      0x00000002
#define SR_CMD_INHIBIT      0x00000001
#define SR_APP_CMD          0x00000020

// INTERRUPT register settings
#define INT_DATA_TIMEOUT    0x00100000
#define INT_CMD_TIMEOUT     0x00010000
#define INT_READ_RDY        0x00000020
#define INT_WRITE_RDY       0x00000010
#define INT_DATA_DONE       0x00000002
#define INT_CMD_DONE        0x00000001

#define INT_ERROR_MASK      0x017E8000

// CONTROL register settings
#define C0_SPI_MODE_EN      0x00100000
#define C0_HCTL_HS_EN       0x00000004
#define C0_HCTL_DWITDH      0x00000002

#define C1_SRST_DATA        0x04000000
#define C1_SRST_CMD         0x02000000
#define C1_SRST_HC          0x01000000
#define C1_TOUNIT_DIS       0x000f0000
#define C1_TOUNIT_MAX       0x000e0000
#define C1_CLK_GENSEL       0x00000020
#define C1_CLK_EN           0x00000004
#define C1_CLK_STABLE       0x00000002
#define C1_CLK_INTLEN       0x00000001

// SLOTISR_VER values
#define HOST_SPEC_NUM       0x00ff0000
#define HOST_SPEC_NUM_SHIFT 16
#define HOST_SPEC_V3        2
#define HOST_SPEC_V2        1
#define HOST_SPEC_V1        0

// SCR flags
#define SCR_SD_BUS_WIDTH_4  0x00000400
#define SCR_SUPP_SET_BLKCNT 0x02000000
// added by my driver
#define SCR_SUPP_CCS        0x00000001

#define ACMD41_VOLTAGE      0x00ff8000
#define ACMD41_CMD_COMPLETE 0x80000000
#define ACMD41_CMD_CCS      0x40000000
#define ACMD41_ARG_HC       0x51ff8000

//unsigned long sd_scr[2], sd_ocr, sd_err, sd_hv; // unsigned long is of size 4
uint64_t sd_scr[2], sd_ocr, sd_rca, sd_err, sd_hv; // grr no, not uint64_t
uint64_t sd_rca;
//unsigned long sd_rca;

/**
 * Wait for data or command ready
 */
int sd_status(unsigned int mask)
{
	int cnt = 1000000; 
	while((*EMMC_STATUS & mask) && !(*EMMC_INTERRUPT & INT_ERROR_MASK) && cnt--) wait_usec(1);
	return (cnt <= 0 || (*EMMC_INTERRUPT & INT_ERROR_MASK)) ? SD_ERROR : SD_OK;
}

/**
 * Wait for interrupt
 */
int sd_int(unsigned int mask)
{
	unsigned int r, m=mask | INT_ERROR_MASK;
	int cnt = 1000000; 
	//cnt *=10; // mcarter boosted, but didn't help
	while(!(*EMMC_INTERRUPT & m) && cnt--) wait_usec(1);
	r=*EMMC_INTERRUPT;
	if(cnt<=0 || (r & INT_CMD_TIMEOUT) || (r & INT_DATA_TIMEOUT) ) { 
		*EMMC_INTERRUPT=r; 
		return SD_TIMEOUT; 
	} else if(r & INT_ERROR_MASK) { 
		*EMMC_INTERRUPT=r; 
		return SD_ERROR; 
	}
	*EMMC_INTERRUPT=mask;
	return 0;
}

/**
 * Send a command
 */
int sd_cmd(unsigned int code, unsigned int arg)
{
	int r=0;
	sd_err=SD_OK;
	if(code&CMD_NEED_APP) {
		r=sd_cmd(CMD_APP_CMD|(sd_rca?CMD_RSPNS_48:0),sd_rca);
		if(sd_rca && !r) { 
			sd_error_puts("ERROR: failed to send SD APP command\n"); 
			sd_err=SD_ERROR;
			return 0;
		}
		code &= ~CMD_NEED_APP;
	}
	if(sd_status(SR_CMD_INHIBIT)) { 
		sd_error_puts("ERROR: EMMC busy\n"); 
		sd_err= SD_TIMEOUT;
		return 0;
	}
	if(code==0x29020000) puts("EMMC: this printf statement needs to be here"); // or it won't work
	if(code != CMD_READ_SINGLE)
		sd_uart_puts("EMMC: Sending command 0x%x (%s), arg 0x%x\n", code, describe_cmd(code), arg);
	*EMMC_INTERRUPT=*EMMC_INTERRUPT; 
	//*EMMC_INTERRUPT= 1; // changed by mcarter 2021-08-20
	EMMC_ARG1=arg; 
	EMMC_CMDTM=code;
	if(code==CMD_SEND_OP_COND) wait_usec(1000);
	if(code==CMD_SEND_IF_COND || code==CMD_APP_CMD) wait_usec(100);
	if((r=sd_int(INT_CMD_DONE))) {
		sd_error_puts("ERROR: failed to send EMMC command\n");
		sd_err=r;
		return 0;
	}
	r=EMMC_RESP0;
	if(code==CMD_GO_IDLE || code==CMD_APP_CMD) return 0;
	if(code==(CMD_APP_CMD|CMD_RSPNS_48)) return r&SR_APP_CMD;
	if(code==CMD_SEND_OP_COND) return r;
	if(code==CMD_SEND_IF_COND) return r==arg? SD_OK : SD_ERROR; 
	if(code==CMD_ALL_SEND_CID) {
		r|=EMMC_RESP3; 
		r|=EMMC_RESP2; 
		r|=EMMC_RESP1; 
		return r; 
	} 
	if(code==CMD_SEND_REL_ADDR) {
		sd_err=(((r&0x1fff))|((r&0x2000)<<6)|((r&0x4000)<<8)|((r&0x8000)<<8))&CMD_ERRORS_MASK;
		return r&CMD_RCA_MASK;
	}
	return r&CMD_ERRORS_MASK;
	// make gcc happy
	return 0;
}

/**
 * read a block from sd card and return the number of bytes read
 * returns 0 on error.
 */
int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num)
{
	int r,c=0,d;
	if(num<1) num=1;
	//printf("\nsd_readblock: lba = 0x%x, num = 0x%x\n", lba, num);
	if(sd_status(SR_DAT_INHIBIT)) {sd_err=SD_TIMEOUT; return 0;}
	unsigned int *buf=(unsigned int *)buffer;
	if(sd_scr[0] & SCR_SUPP_CCS) {
		if(num > 1 && (sd_scr[0] & SCR_SUPP_SET_BLKCNT)) {
			sd_cmd(CMD_SET_BLOCKCNT,num);
			if(sd_err) return 0;
		}
		*EMMC_BLKSIZECNT = (num << 16) | 512;
		sd_cmd(num == 1 ? CMD_READ_SINGLE : CMD_READ_MULTI,lba);
		if(sd_err) return 0;
	} else {
		*EMMC_BLKSIZECNT = (1 << 16) | 512;
	}
	while( c < num ) {
		if(!(sd_scr[0] & SCR_SUPP_CCS)) {
			sd_cmd(CMD_READ_SINGLE,(lba+c)*512);
			if(sd_err) return 0;
		}
		wait_usec(1000); // mcarter 2021-08-21 to avoid timeout. didn't help
		if((r=sd_int(INT_READ_RDY))){
			sd_error_puts("\rERROR: Timeout waiting for ready to read\n");
			sd_err=r;
			return 0;
		}
		for(d=0;d<128;d++) buf[d] = *EMMC_DATA;
		c++; buf+=128;
	}
	if( num > 1 && !(sd_scr[0] & SCR_SUPP_SET_BLKCNT) && (sd_scr[0] & SCR_SUPP_CCS)) sd_cmd(CMD_STOP_TRANS,0);
	return sd_err!=SD_OK || c!=num? 0 : num*512;
}

int readablock(unsigned int lba, unsigned char *buffer)
{
    int status = sd_readblock(lba, buffer, 1);
    if(status == 0) return -1; // an error
    return 0;
}

/**
 * write a block to the sd card and return the number of bytes written
 * returns 0 on error.
 */
int sd_writeblock(unsigned char *buffer, unsigned int lba, unsigned int num)
{
	int r,c=0,d;
	if(num<1) num=1;
	sd_uart_puts("sd_writeblock lba ");sd_uart_hex(lba);sd_uart_puts(" num ");sd_uart_hex(num);sd_uart_puts("\n");
	if(sd_status(SR_DAT_INHIBIT | SR_WRITE_AVAILABLE)) {sd_err=SD_TIMEOUT; return 0;}
	unsigned int *buf=(unsigned int *)buffer;
	if(sd_scr[0] & SCR_SUPP_CCS) {
		if(num > 1 && (sd_scr[0] & SCR_SUPP_SET_BLKCNT)) {
			sd_cmd(CMD_SET_BLOCKCNT,num);
			if(sd_err) return 0;
		}
		*EMMC_BLKSIZECNT = (num << 16) | 512;
		sd_cmd(num == 1 ? CMD_WRITE_SINGLE : CMD_WRITE_MULTI,lba);
		if(sd_err) return 0;
	} else {
		*EMMC_BLKSIZECNT = (1 << 16) | 512;
	}
	while( c < num ) {
		if(!(sd_scr[0] & SCR_SUPP_CCS)) {
			sd_cmd(CMD_WRITE_SINGLE,(lba+c)*512);
			if(sd_err) return 0;
		}
		if((r=sd_int(INT_WRITE_RDY))){
			sd_error_puts("\rERROR: Timeout waiting for ready to write\n");
			sd_err=r;
			return 0;
		}
		for(d=0;d<128;d++) *EMMC_DATA = buf[d];
		c++; buf+=128;
	}
	if((r=sd_int(INT_DATA_DONE))){
		sd_error_puts("\rERROR: Timeout waiting for data done\n");
		sd_err=r;
		return 0;
	}
	if( num > 1 && !(sd_scr[0] & SCR_SUPP_SET_BLKCNT) && (sd_scr[0] & SCR_SUPP_CCS)) sd_cmd(CMD_STOP_TRANS,0);
	return sd_err!=SD_OK || c!=num? 0 : num*512;
}

/**
 * set SD clock to frequency in Hz
 */
int sd_clk(unsigned int f)
{
	unsigned int d,c=41666666/f,x,s=32,h=0;
	int cnt = 100000;
	while((*EMMC_STATUS & (SR_CMD_INHIBIT|SR_DAT_INHIBIT)) && cnt--) wait_usec(1);
	if(cnt<=0) {
		sd_error_puts("ERROR: timeout waiting for inhibit flag\n");
		return SD_ERROR;
	}

	*EMMC_CONTROL1 &= ~C1_CLK_EN; wait_usec(10);
	x=c-1; if(!x) s=0; else {
		if(!(x & 0xffff0000u)) { x <<= 16; s -= 16; }
		if(!(x & 0xff000000u)) { x <<= 8;  s -= 8; }
		if(!(x & 0xf0000000u)) { x <<= 4;  s -= 4; }
		if(!(x & 0xc0000000u)) { x <<= 2;  s -= 2; }
		if(!(x & 0x80000000u)) { x <<= 1;  s -= 1; }
		if(s>0) s--;
		if(s>7) s=7;
	}
	if(sd_hv>HOST_SPEC_V2) d=c; else d=(1<<s);
	if(d<=2) {d=2;s=0;}
	sd_uart_puts("sd_clk divisor ");sd_uart_hex(d);sd_uart_puts(", shift ");sd_uart_hex(s);sd_uart_puts("\n");
	if(sd_hv>HOST_SPEC_V2) h=(d&0x300)>>2;
	d=(((d&0x0ff)<<8)|h);
	*EMMC_CONTROL1=(*EMMC_CONTROL1&0xffff003f)|d; wait_usec(10);
	*EMMC_CONTROL1 |= C1_CLK_EN; wait_usec(10);
	cnt=10000; while(!(*EMMC_CONTROL1 & C1_CLK_STABLE) && cnt--) wait_usec(10);
	if(cnt<=0) {
		sd_error_puts("ERROR: failed to get stable clock\n");
		return SD_ERROR;
	}
	return SD_OK;
}

/**
 * initialize EMMC to read SDHC card
 */


int sdcard_init1()
{
	printf("sizeof(unsigned long)=%d\n", sizeof(unsigned long));
	printf("sizeof(int)=%d\n", sizeof(int));
	//long r,cnt,ccs=0;
	int64_t r,cnt,ccs=0;
	// GPIO_CD
	r=GPFSEL4; r&=~(7<<(7*3)); GPFSEL4=r;
	GPPUD=2; wait_cycles(150); GPPUDCLK1=(1<<15); wait_cycles(150); GPPUD=0; GPPUDCLK1=0;
	r=GPHEN1; r|=1<<15; GPHEN1=r;

	// GPIO_CLK, GPIO_CMD
	r=GPFSEL4; r|=(7<<(8*3))|(7<<(9*3)); GPFSEL4=r;
	GPPUD=2; wait_cycles(150); GPPUDCLK1=(1<<16)|(1<<17); wait_cycles(150); GPPUD=0; GPPUDCLK1=0;

	// GPIO_DAT0, GPIO_DAT1, GPIO_DAT2, GPIO_DAT3
	r=GPFSEL5; r|=(7<<(0*3)) | (7<<(1*3)) | (7<<(2*3)) | (7<<(3*3)); GPFSEL5=r;
	GPPUD=2; wait_cycles(150);
	GPPUDCLK1=(1<<18) | (1<<19) | (1<<20) | (1<<21);
	wait_cycles(150); GPPUD=0; GPPUDCLK1=0;

	sd_hv = (*EMMC_SLOTISR_VER & HOST_SPEC_NUM) >> HOST_SPEC_NUM_SHIFT;
	//alternate way
	sd_hv = *EMMC_SLOTISR_VER;
	sd_hv >>=16;
	sd_hv &= 0xFF;
	sd_uart_puts("EMMC: GPIO set up. Card version is %d", sd_hv);
	sd_uart_hex(sd_hv);
	sd_uart_puts("Number above is card version"); // I think there's something wrong my printing
	// Reset the card.
	*EMMC_CONTROL0 = 0; *EMMC_CONTROL1 |= C1_SRST_HC;
	cnt=10000; do{wait_usec(10);} while( (*EMMC_CONTROL1 & C1_SRST_HC) && cnt-- );
	if(cnt<=0) {
		sd_error_puts("ERROR: failed to reset EMMC\n");
		return SD_ERROR;
	}
	sd_uart_puts("EMMC: reset OK\n");
	*EMMC_CONTROL1 |= C1_CLK_INTLEN | C1_TOUNIT_MAX;
	wait_usec(10);
	// Set clock to setup frequency.
	if((r=sd_clk(400000))) return r;
	*EMMC_INT_EN   = 0xffffffff;
	*EMMC_INT_MASK = 0xffffffff;
	sd_scr[0]=sd_scr[1]=sd_rca=sd_err=0;
	sd_cmd(CMD_GO_IDLE,0);
	if(sd_err) return sd_err;

#if 0
	// determine card version
	u32 response;
	puts("detemine card version");
	puts("  wait for ready");
	if(sd_status(SR_CMD_INHIBIT)) { 
		sd_error_puts("ERROR: EMMC busy\n"); 
		sd_err= SD_TIMEOUT;
		return 0;
	}
	puts("  in ready in state");
	//*EMMC_INTERRUPT=*EMMC_INTERRUPT;
	*EMMC_INTERRUPT= 1;
	EMMC_ARG1 = 0x1AA; // CMD8
	//EMMC_ARG1 = 8; // CMD8
	//EMMC_ARG1 = CMD_SEND_IF_COND; // CMD8
	EMMC_CMDTM = CMD_SEND_IF_COND;
	//EMMC_CMDTM=(0x40 | 8) <<24; // CMD8
	//EMMC_CMDTM=8 ; // CMD8
	wait_usec(100);
	puts("  wait for cmd done");
	if((r=sd_int(INT_CMD_DONE))) {
		sd_error_puts("ERROR: failed to send EMMC command\n");
		sd_err=r;
		return 0;
	}
	puts("  cmd is done");
	response = EMMC_RESP0;
	printf("  response= %d, r=%d\n", response, r);
	int card_version = -1; 
	if(r == R1_IDLE_STATE)
		card_version = 2;
	else if(r == (R1_IDLE_STATE | R1_ILLEGAL_COMMAND))
		card_version = 1;
	printf("Card version% %d\n", card_version);

	printf("finished determinging card version");
#endif



	
	sd_cmd(CMD_SEND_IF_COND,0x000001AA);
	if(sd_err) return sd_err;
	cnt=6; r=0; while(!(r&ACMD41_CMD_COMPLETE) && cnt--) {
		wait_cycles(400);
		r=sd_cmd(CMD_SEND_OP_COND,ACMD41_ARG_HC);
		sd_uart_puts("EMMC: CMD_SEND_OP_COND returned ");
		if(r&ACMD41_CMD_COMPLETE)
			sd_uart_puts("COMPLETE ");
		if(r&ACMD41_VOLTAGE)
			sd_uart_puts("VOLTAGE ");
		if(r&ACMD41_CMD_CCS)
			sd_uart_puts("CCS ");
		sd_uart_hex(r>>32);
		sd_uart_hex(r);
		sd_uart_puts("\n");
		if(sd_err!=SD_TIMEOUT && sd_err!=SD_OK ) {
			sd_error_puts("ERROR: EMMC ACMD41 returned error\n");
			return sd_err;
		}
	}
	if(!(r&ACMD41_CMD_COMPLETE) || !cnt ) return SD_TIMEOUT;
	if(!(r&ACMD41_VOLTAGE)) return SD_ERROR;
	if(r&ACMD41_CMD_CCS) ccs=SCR_SUPP_CCS;

	sd_cmd(CMD_ALL_SEND_CID,0);

	sd_rca = sd_cmd(CMD_SEND_REL_ADDR,0);
	sd_uart_puts("EMMC: CMD_SEND_REL_ADDR returned ");
	sd_uart_hex(sd_rca>>32);
	sd_uart_hex(sd_rca);
	sd_uart_puts("\n");
	if(sd_err) return sd_err;

	if((r=sd_clk(25000000))) return r;

	sd_cmd(CMD_CARD_SELECT,sd_rca);
	if(sd_err) return sd_err;

	if(sd_status(SR_DAT_INHIBIT)) return SD_TIMEOUT;
	*EMMC_BLKSIZECNT = (1<<16) | 8;
	sd_cmd(CMD_SEND_SCR,0);
	if(sd_err) return sd_err;
	if(sd_int(INT_READ_RDY)) return SD_TIMEOUT;

	r=0; cnt=100000; while(r<2 && cnt) {
		if( *EMMC_STATUS & SR_READ_AVAILABLE )
			sd_scr[r++] = *EMMC_DATA;
		else
			wait_usec(1);
	}
	if(r!=2) return SD_TIMEOUT;
	if(sd_scr[0] & SCR_SD_BUS_WIDTH_4) {
		puts("setting bus width");
		sd_cmd(CMD_SET_BUS_WIDTH,sd_rca|2);
		if(sd_err) return sd_err;
		*EMMC_CONTROL0 |= C0_HCTL_DWITDH;
	} else {
		puts("no need to set bus width");
	}

	// add software flag
	sd_uart_puts("EMMC: supports ");
	if(sd_scr[0] & SCR_SUPP_SET_BLKCNT)
		sd_uart_puts("SET_BLKCNT ");
	if(ccs)
		sd_uart_puts("CCS ");
	sd_uart_puts("\n");
	sd_scr[0]&=~SCR_SUPP_CCS;
	sd_scr[0]|=ccs;
	return SD_OK;
}

void sdcard_init(void)
{
	int status = sdcard_init1(); // perhaps needs some nice handling
}

void sdcard_deinit(void)
{
	// should probably do something
}

