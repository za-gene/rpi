/*
 * Memory-to-memory must use DMA2
 *
 * The F4 has 2 controllers: DMA1, and DMA2.
 * Each DMA has 8 streams: pathways where memory can flow
 *
 * Further info:
 * https://adammunich.com/stm32-dma-cheat-sheet/
 */

#include <libopencm3/stm32/rcc.h>
//#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/dma.h>

#include <string.h>

#include "mal.h"

typedef uint32_t u32;

#define LED PC13

uint32_t dma = DMA2;
//uint8_t ch = -3;
uint8_t strm = DMA_STREAM0; // chosen aritrarily

static void myputs(const char *str)
{
	mal_usart_print(str);
	mal_usart_print("\r\n");
}


volatile bool transfer_complete = false;

void dma2_stream0_isr()
{
	if(dma_get_interrupt_flag(dma, strm, DMA_TCIF)) {
		dma_clear_interrupt_flags(dma, strm, DMA_TCIF); // clear transfer complete flag
		transfer_complete = true;
		myputs("dma2_stream0_isr called: transfer complete");
	} 
	else if(dma_get_interrupt_flag(dma, strm, DMA_DMEIF)) {
		dma_clear_interrupt_flags(dma, strm, DMA_DMEIF);
		myputs("dma2_stream0_isr called: Direct Mode Error Interrupt Flag");
	} 
	else if(dma_get_interrupt_flag(dma, strm, DMA_TEIF)) {
		dma_clear_interrupt_flags(dma, strm, DMA_TEIF);
		myputs("dma2_stream0_isr called: Transfer Error Interrupt Flag");
	} 
	else if(dma_get_interrupt_flag(dma, strm, DMA_FEIF)) {
		dma_clear_interrupt_flags(dma, strm, DMA_FEIF);
		myputs("dma2_stream0_isr called: FIFO Error Interrupt Flag");
	} 
	else if(dma_get_interrupt_flag(dma, strm, DMA_HTIF)) {
		dma_clear_interrupt_flags(dma, strm, DMA_HTIF);
		myputs("dma2_stream0_isr called: Half Transfer Interrupt Flag");
	} 
	else {
                myputs("dma2_stream0_isr called: Unhandled (should never be called)");
	}

}

int main(void)
{
	pin_out(LED);
	mal_usart_init();

	myputs("");
	myputs("=============================");
	myputs("DMA example: memory to memory");

	char src1[] = "1234567890";
	char dst1[] = "abcdefghij";
	uint32_t len = strlen(src1) + 1;

	myputs(dst1);

#if 1
	// follow instructions in s9.3.17 of RM0383a, p181
	rcc_periph_clock_enable(RCC_DMA2);

	// step 1 : disable the stream
	myputs("Disabling stream");
	//dma_disable_stream(dma, strm);
	DMA_SCR(dma, strm) &= ~DMA_SxCR_EN;
	while(DMA_SCR(dma, strm) & DMA_SxCR_EN); // wait until it is down
	myputs("Stream disabled. OK to configure");

	//DMA_SPAR(dma, strm) = (uint32_t) str1; // step 2: set peripheral port address
	myputs("Step 2");
	//dma_set_peripheral_address(dma, strm, (uint32_t) src1); // step 2: set source address
	//DMA_SM0AR(dma, strm) = *(uint32_t*) str2; // step 3: set the memory address
	myputs("Step 3");
	dma_set_memory_address(dma, strm, (uint32_t) dst1); // step 3 : destination address
	myputs("Step 4");
	dma_set_number_of_data(dma, strm, len); // step 4: total number of data items
	// dma_channel_select(dma, strm, 0); // step 5: I just made up a channel number in this case
	myputs("Step 5");
	dma_channel_select(dma, strm, DMA_SxCR_CHSEL_4); // step 5: I just made up a channel number in this case
	// step 6: something about flow controller. Omitted
	// step 7: configure stream priority. Omitted
	// step 8: configure FIFO usage. Omitted setup of FIFO

	// step 9: variety of things
	myputs("Step 9a");
	dma_set_transfer_mode(dma, strm, DMA_SxCR_DIR_MEM_TO_MEM); // step 9: data direction
	myputs("Step 9b");
	dma_enable_peripheral_increment_mode(dma, strm); // step 9: we want to increment "periph" address
	myputs("Step 9c");
	dma_enable_memory_increment_mode(dma, strm); // step 9: ditto for memory
	//dma_enable_directt_mode(dma, strm); // step 9: 
	// step 9 : can use single or burst, but not circ, direct or double-buffer
	myputs("Step 9d");
	dma_set_memory_size(dma, strm, DMA_SxCR_MSIZE_8BIT);
	myputs("Step 9e");
	//dma_set_peripheral_size(dma, strm, len);
	dma_set_peripheral_size(dma, strm, DMA_SxCR_PSIZE_8BIT);
	myputs("Fiddling with interrupts");
	//dma_clear_interrupt_flags(dma, strm, DMA_TCIF); // clear transfer complete flag
	//dma_clear_interrupt_flags(dma, strm, DMA_HTIF); // clear half-transfer complete flag
	//dma_disable_half_transfer_interrupt(dma, strm);
	dma_enable_transfer_complete_interrupt(dma, strm);
	nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
	myputs("Finished setting up interrupts");


	myputs("Start tfr 1");
	transfer_complete = false;
	dma_set_peripheral_address(dma, strm, (uint32_t) src1); // step 2: set source address
	dma_enable_stream(dma, strm); // step 10
#endif
	myputs(dst1); // this will likely only partially complete
	while(!transfer_complete);
	myputs("Tfr 1 completed");
	myputs(dst1);
	myputs(src1);

	//dma_enable_stream(dma, strm);



	myputs("\r\nStart tfr 2");
	transfer_complete = false;
	char src2[] = "ABCDEFGHIJ";
	dma_set_peripheral_address(dma, strm, (uint32_t) src2);
	dma_enable_stream(dma, strm);
	while(!transfer_complete);
	myputs(dst1);
	myputs("Tfr 2 completed");

	while(1);

	while (1)
	{
		uint16_t c = usart_recv_blocking(USART1);
		usart_send_blocking(USART1, c);
	}

	while (1)
	{
		pin_toggle(LED);
		mal_delayish(100);
	}
}
