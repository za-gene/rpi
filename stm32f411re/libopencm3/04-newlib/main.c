#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>

#include <ctype.h>
#include <stdio.h>

#include <string.h>

typedef uint32_t u32;


// https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f4/nucleo-f411re/usart-stdio/usart-stdio.c

int _write(int fd, char *ptr, int len);

void get_buffered_line(void);

/*
 * This is a pretty classic ring buffer for characters
 */
#define BUFLEN 127

static uint16_t start_ndx;
static uint16_t end_ndx;
static char buf[BUFLEN+1];
#define buf_len ((end_ndx - start_ndx) % BUFLEN)
static inline int inc_ndx(int n) { return ((n + 1) % BUFLEN); }
static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }


/* back up the cursor one space */
static inline void back_up(void)
{
	end_ndx = dec_ndx(end_ndx);
	usart_send_blocking(USART1, '\010');
	usart_send_blocking(USART1, ' ');
	usart_send_blocking(USART1, '\010');
}

/*
 * A buffered line editing function.
 */
void
get_buffered_line(void) {
	char	c;

	if (start_ndx != end_ndx) {
		return;
	}
	while (1) {
		c = usart_recv_blocking(USART1);
		if (c == '\r') {
			buf[end_ndx] = '\n';
			end_ndx = inc_ndx(end_ndx);
			buf[end_ndx] = '\0';
			usart_send_blocking(USART1, '\r');
			usart_send_blocking(USART1, '\n');
			return;
		}
		/* ^H or DEL erase a character */
		if ((c == '\010') || (c == '\177')) {
			if (buf_len == 0) {
				usart_send_blocking(USART1, '\a');
			} else {
				back_up();
			}
		/* ^W erases a word */
		} else if (c == 0x17) {
			while ((buf_len > 0) &&
					(!(isspace((int) buf[end_ndx])))) {
				back_up();
			}
		/* ^U erases the line */
		} else if (c == 0x15) {
			while (buf_len > 0) {
				back_up();
			}
		/* Non-editing character so insert it */
		} else {
			if (buf_len == (BUFLEN - 1)) {
				usart_send_blocking(USART1, '\a');
			} else {
				buf[end_ndx] = c;
				end_ndx = inc_ndx(end_ndx);
				usart_send_blocking(USART1, c);
			}
		}
	}
}


/*
 * Called by the libc stdio fread fucntions
 *
 * Implements a buffered read with line editing.
 */
int _read(int fd, char *ptr, int len);

int _read(int fd, char *ptr, int len)
{
	int	my_len;

	if (fd > 2) {
		return -1;
	}

	get_buffered_line();
	my_len = 0;
	while ((buf_len > 0) && (len > 0)) {
		*ptr++ = buf[start_ndx];
		start_ndx = inc_ndx(start_ndx);
		my_len++;
		len--;
	}
	return my_len; /* return the length we got */
}


int _write(int fd, char *ptr, int len)
{
	int i = 0;

	/*
	 * Write "len" of char from "ptr" to file id "fd"
	 * Return number of char written.
	 *
	 * Only work for STDOUT, STDIN, and STDERR
	 */
	if (fd > 2) {
		return -1;
	}
	while (*ptr && (i < len)) {
		usart_send_blocking(USART1, *ptr);
		if (*ptr == '\n') {
			usart_send_blocking(USART1, '\r');
		}
		i++;
		ptr++;
	}
	return i;
}


int getchar(void)
{
	return usart_recv_blocking(USART1);
}
int putchar(int c)
{
	usart_send_blocking(USART1, c);
	return (unsigned char)c;
}

int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO9); // TX
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO10); // RX
	gpio_set_af(GPIOA, GPIO_AF7, GPIO10);
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);

	char msg[] = "I will echo everything you type...\r\n";
	char *str = msg;
	while(*str) usart_send_blocking(USART1, *str++);

	char copy[100];
	memcpy(copy, msg, strlen(msg)); // let's see if we can copy a string using newlib
	printf("This string printed using the %s command\r\n", "printf()");
	sprintf(copy, "The meaning of of is %d\r\n", 42);
	str = copy;
	while(*str) usart_send_blocking(USART1, *str++);
	puts("This is a test of puts()");


	char local_buf[32];
	printf("Enter some text, pressing return afterwards :");
	fflush(stdout);
	fgets(local_buf, 32, stdin);
	printf("You entered '%s'\r\n", local_buf);

	printf("I will echo everything you type...\r\n");


	while (1)
	{
		//uint16_t c = usart_recv_blocking(USART1);
		int c = getchar();
		//usart_send_blocking(USART1, c);
		putchar(c);
	}
}
