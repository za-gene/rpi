#include <gpio.h>
#include <stdio.h>
#include <timers.h>

// DMA control block (must be 32-byte aligned)
typedef struct __attribute__((packed)) {
    	uint32_t ti,    // Transfer info
        src,    	// Source address
        dest,    	// Destination address
        tfr_len,    	// Transfer length
        stride,     	// Transfer stride
        next_cb,    	// Next control block
	reserved[2];	// reserved. I don't think you need to set them.
} dma_cb_t __attribute__ ((aligned(256))); // original code said 32
#define DMA_CB_DEST_INC (1<<4)
#define DMA_CB_SRC_INC  (1<<8)



// DMA Control Register
typedef struct {
    uint32_t cs;      // DMA Channel Control and Status register
    uint32_t cb_addr; // DMA Channel Control Block Address
} dma_cr_t;

/*
// DMA Memory Handle
typedef struct {
    void *virtual_addr; // Virutal base address of the page
    uint32_t bus_addr;  // Bus adress of the page, this is not a pointer because it does not point to valid virtual address
    uint32_t mb_handle; // Used by mailbox property interface
    uint32_t size;
} dma_mh_t;
*/


/* According to:
 * https://github.com/AZO234/RaspberryPi_BareMetal/blob/master/rp_framebuffer_dma1/rp_baremetal.h
 * which seems correct
 */
#define DMA_BASE (PBASE+0x7000) 
// The following settings appear to be wrong
//#define DMA_BASE 0x7E007000 // ?? At least that's what BCM2835 docs says
//#define DMA_BASE 0x007000 

// see pages 47 and 58
#define DMA_INT_STATUS 	REG(DMA_BASE+0xFE0)
#define DMA_ENABLE 	REG(DMA_BASE+0xFF0)

/* DMA CS Control and Status bits */
#define DMA_CHANNEL_RESET (1 << 31)
#define DMA_CHANNEL_ABORT (1 << 30)
#define DMA_WAIT_ON_WRITES (1 << 28)
#define DMA_PANIC_PRIORITY(x) ((x) << 20)
#define DMA_PRIORITY(x) ((x) << 16)
#define DMA_INTERRUPT_STATUS (1 << 2)
#define DMA_END_FLAG (1 << 1)
#define DMA_ACTIVE (1 << 0)
#define DMA_DISDEBUG (1 << 28)



/*
 * Pi2 and 3 use region 0xC0000000 for the bus address, to do do a conversion
 * DMASourcePtr = myBufferPtr or 0xC0000000
 * DMADestPtr = GPUBufferPtr or 0xC0000000
 * To convert from a bus address to a physical address:
 *  myBufferPtr = DMASourcePtr and not(0xC0000000)
 *  Source: https://www.raspberrypi.org/forums/viewtopic.php?p=973584
 *
 * See basal.h for the DMA region
 */

#define DMA_BUS_TO_PHYS(addr) ((uint32_t)(addr) & ~GPU_MEM_BASE)
#define DMA_PHYS_TO_BUS(addr) ((uint32_t)(addr) | GPU_MEM_BASE)
#define DMA_PERF_TO_BUS(x) ((uint32_t)(x) | 0x7E000000) // prolly for pi 3, not 0

void kernel_main(void)
{
	puts("\nDMA test: memory to memory");

	char src[] = "hello";
	int size = sizeof(src);
	printf("size = %d\n", size);

	char dest[size];
	dest[0] = 0;
	

	//goto skip;
	const int dma_channel = 0;
	DMA_ENABLE |= (1<<dma_channel);
	//volatile dma_cb_t *cb = (volatile dma_cb_t*)(DMA_BASE +0x8 +dma_channel * 0x100); // but channel 15 is special
	volatile __attribute__ ((aligned (32)))  dma_cb_t cb;
	//dma_cb_t cb;
	cb.ti = DMA_CB_SRC_INC | DMA_CB_DEST_INC;
	cb.src = DMA_PHYS_TO_BUS(src);
	//cb.src = src;
	cb.dest = DMA_PHYS_TO_BUS(dest);
	//cb.dest = dest;
	cb.tfr_len = size;
	cb.stride = 0;
	cb.next_cb = 0;
	cb.reserved[0] = 0;
	cb.reserved[1] = 0;

	// start the dma
	volatile dma_cr_t *cr = (volatile dma_cr_t*)(DMA_BASE +dma_channel * 0x100); // but channel 15 is special
	// reset the dma channel
	cr->cs = DMA_CHANNEL_ABORT;
	cr->cs = 0;
	cr->cs = DMA_CHANNEL_RESET;
	cr->cb_addr = 0;
	cr->cs = DMA_INTERRUPT_STATUS | DMA_END_FLAG;
	// now enable it
	//cr->cb_addr = DMA_PHYS_TO_BUS(&cb);
	cr->cb_addr = (uint32_t)&cb;
	//cr->cb_addr = &cb;
	//cr->cs |= DMA_WAIT_ON_WRITES | DMA_ACTIVE;
	cr->cs = 0x1;

//skip:
	//delay_ms(250);
	while((cr->cs & 0x2) == 0); // wait for transfer complete
	cr->cs = 0x2; // clear end of transfer

	printf("dest=%s\n", dest);
	puts("that's all folks");





	const int bcm_pin = 26;
	gpio_sel(bcm_pin, OUTPUT); // set its pin mode to OUTPUT
	u32 delay = 250;
	while(1) {
		gpio_set(bcm_pin); // set the pin high
		delay_ms(delay);  
		gpio_clr(bcm_pin); // turn the pin off
		delay_ms(delay);
	}
}
