#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <timers.h>

typedef uint8_t rt_uint8_t;
typedef int32_t rt_int32_t;
typedef uint16_t rt_uint16_t;
typedef uint32_t rt_uint32_t;


struct rt_mutex { int dummy; }; // never actually used
struct rt_semaphore { int dummy; }; // never actually used

// see rt-thread/include/rtdef.h
typedef long                            rt_base_t;      /**< Nbit CPU related date type */
typedef rt_base_t                       rt_err_t;       /**< Type for error number */
#define RT_EOK	0
#define RT_EIO	8  
#define RT_NULL (0)
#define RT_ERROR 1
#define RT_ETIMEOUT                     2   // timed out
#define RT_EINVAL                       10  // invalid argument

// see components/drivers/include/drivers/mmcsd_cmd.h
#define SD_APP_OP_COND           41   /* bcr  [31:0] OCR         R3  */
#define SET_BLOCK_COUNT      23   /* adtc [31:0] data addr   R1  */
#define SD_SEND_IF_COND           8   /* bcr  [11:0] See below   R7  */
#define APP_CMD              55   /* ac   [31:16] RCA        R1  */


// see components/drivers/include/drivers/mmcsd_core.h
struct rt_mmcsd_data {
	rt_uint32_t  blksize;
	rt_uint32_t  blks;
	rt_uint32_t  *buf;
	rt_int32_t  err;
	rt_uint32_t  flags;
#define DATA_DIR_WRITE  (1 << 0)
#define DATA_DIR_READ   (1 << 1)
#define DATA_STREAM (1 << 2)

	unsigned int        bytes_xfered;

	struct rt_mmcsd_cmd *stop;      /* stop command */
	struct rt_mmcsd_req *mrq;       /* associated request */

	rt_uint32_t  timeout_ns;
	rt_uint32_t  timeout_clks;
};
#define RESP_MASK   (0xF)
#define RESP_R1     (1 << 0)
#define RESP_R2     (3 << 0)
struct rt_mmcsd_cmd {
	rt_uint32_t  cmd_code;
	rt_uint32_t  arg;
	rt_uint32_t  resp[4];
	rt_uint32_t  flags;
	/*rsponse types
	 *bits:0~3
	 */
#define RESP_MASK   (0xF)
#define RESP_NONE   (0)
#define RESP_R1     (1 << 0)
#define RESP_R1B    (2 << 0)
#define RESP_R2     (3 << 0)
#define RESP_R3     (4 << 0)
#define RESP_R4     (5 << 0)
#define RESP_R6     (6 << 0)
#define RESP_R7     (7 << 0)
#define RESP_R5     (8 << 0)    /*SDIO command response type*/
	/*command types
	 *bits:4~5
	 */
#define CMD_MASK    (3 << 4)        /* command type */
#define CMD_AC      (0 << 4)
#define CMD_ADTC    (1 << 4)
#define CMD_BC      (2 << 4)
#define CMD_BCR     (3 << 4)

#define resp_type(cmd)  ((cmd)->flags & RESP_MASK)

	/*spi rsponse types
	 *bits:6~8
	 */
#define RESP_SPI_MASK   (0x7 << 6)
#define RESP_SPI_R1 (1 << 6)
#define RESP_SPI_R1B    (2 << 6)
#define RESP_SPI_R2 (3 << 6)
#define RESP_SPI_R3 (4 << 6)
#define RESP_SPI_R4 (5 << 6)
#define RESP_SPI_R5 (6 << 6)
#define RESP_SPI_R7 (7 << 6)

#define spi_resp_type(cmd)  ((cmd)->flags & RESP_SPI_MASK)
	/*
	 * These are the command types.
	 */
#define cmd_type(cmd)   ((cmd)->flags & CMD_MASK)

	rt_int32_t  retries;    /* max number of retries */
	rt_int32_t  err;

	struct rt_mmcsd_data *data;
	struct rt_mmcsd_req *mrq;       /* associated request */
};
struct rt_mmcsd_req {
	struct rt_mmcsd_data  *data;
	struct rt_mmcsd_cmd   *cmd;
	struct rt_mmcsd_cmd   *stop;
};

rt_uint32_t __rt_fls(rt_uint32_t val);

// see components/drivers/include/drivers/mmcsd_host.h
#define MMCSD_BUS_WIDTH_4       2
struct rt_mmcsd_io_cfg {
	rt_uint32_t clock;          /* clock rate */
	rt_uint16_t vdd;

	/* vdd stores the bit number of the selected voltage range from below. */

	rt_uint8_t  bus_mode;       /* command output mode */

#define MMCSD_BUSMODE_OPENDRAIN 1
#define MMCSD_BUSMODE_PUSHPULL  2

	rt_uint8_t  chip_select;        /* SPI chip select */

#define MMCSD_CS_IGNORE     0
#define MMCSD_CS_HIGH       1
#define MMCSD_CS_LOW        2

	rt_uint8_t  power_mode;     /* power supply mode */

#define MMCSD_POWER_OFF     0
#define MMCSD_POWER_UP      1
#define MMCSD_POWER_ON      2

	rt_uint8_t  bus_width;      /* data bus width */

#define MMCSD_BUS_WIDTH_1       0
#define MMCSD_BUS_WIDTH_4       2
#define MMCSD_BUS_WIDTH_8       3

};
struct rt_mmcsd_host;
struct rt_mmcsd_req;
struct rt_mmcsd_host_ops {
	void (*request)(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req);
	void (*set_iocfg)(struct rt_mmcsd_host *host, struct rt_mmcsd_io_cfg *io_cfg);
	rt_int32_t (*get_card_status)(struct rt_mmcsd_host *host);
	void (*enable_sdio_irq)(struct rt_mmcsd_host *host, rt_int32_t en);
};
struct rt_mmcsd_host {
	struct rt_mmcsd_card *card;
	const struct rt_mmcsd_host_ops *ops;
	rt_uint32_t  freq_min;
	rt_uint32_t  freq_max;
	struct rt_mmcsd_io_cfg io_cfg;
	rt_uint32_t  valid_ocr; /* current valid OCR */
#define VDD_165_195     (1 << 7)    /* VDD voltage 1.65 - 1.95 */
#define VDD_20_21       (1 << 8)    /* VDD voltage 2.0 ~ 2.1 */
#define VDD_21_22       (1 << 9)    /* VDD voltage 2.1 ~ 2.2 */
#define VDD_22_23       (1 << 10)   /* VDD voltage 2.2 ~ 2.3 */
#define VDD_23_24       (1 << 11)   /* VDD voltage 2.3 ~ 2.4 */
#define VDD_24_25       (1 << 12)   /* VDD voltage 2.4 ~ 2.5 */
#define VDD_25_26       (1 << 13)   /* VDD voltage 2.5 ~ 2.6 */
#define VDD_26_27       (1 << 14)   /* VDD voltage 2.6 ~ 2.7 */
#define VDD_27_28       (1 << 15)   /* VDD voltage 2.7 ~ 2.8 */
#define VDD_28_29       (1 << 16)   /* VDD voltage 2.8 ~ 2.9 */
#define VDD_29_30       (1 << 17)   /* VDD voltage 2.9 ~ 3.0 */
#define VDD_30_31       (1 << 18)   /* VDD voltage 3.0 ~ 3.1 */
#define VDD_31_32       (1 << 19)   /* VDD voltage 3.1 ~ 3.2 */
#define VDD_32_33       (1 << 20)   /* VDD voltage 3.2 ~ 3.3 */
#define VDD_33_34       (1 << 21)   /* VDD voltage 3.3 ~ 3.4 */
#define VDD_34_35       (1 << 22)   /* VDD voltage 3.4 ~ 3.5 */
#define VDD_35_36       (1 << 23)   /* VDD voltage 3.5 ~ 3.6 */
	rt_uint32_t  flags; /* define device capabilities */
#define MMCSD_BUSWIDTH_4    (1 << 0)
#define MMCSD_BUSWIDTH_8    (1 << 1)
#define MMCSD_MUTBLKWRITE   (1 << 2)
#define MMCSD_HOST_IS_SPI   (1 << 3)
#define controller_is_spi(host) (host->flags & MMCSD_HOST_IS_SPI)
#define MMCSD_SUP_SDIO_IRQ  (1 << 4)    /* support signal pending SDIO IRQs */
#define MMCSD_SUP_HIGHSPEED (1 << 5)    /* support high speed */

	rt_uint32_t max_seg_size;   /* maximum size of one dma segment */
	rt_uint32_t max_dma_segs;   /* maximum number of dma segments in one request */
	rt_uint32_t max_blk_size;   /* maximum block size */
	rt_uint32_t max_blk_count;  /* maximum block count */

	rt_uint32_t   spi_use_crc;
	struct rt_mutex  bus_lock;
	struct rt_semaphore  sem_ack;

	rt_uint32_t       sdio_irq_num;
	struct rt_semaphore    *sdio_irq_sem;
	struct rt_thread     *sdio_irq_thread;

	void *private_data;
};
void mmcsd_req_complete(struct rt_mmcsd_host *host);


#define DELAY_MICROS delay_us
#define INIT_DEVICE_EXPORT(x)


#define rt_kprintf printf
#define mmcsd_dbg printf

void rt_free(void *rmem);
void *rt_memset(void *s, int c, size_t n);


int raspi_sdmmc_init(void);

