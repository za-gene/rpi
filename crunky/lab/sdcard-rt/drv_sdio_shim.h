#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <timers.h>

typedef uint8_t rt_uint8_t;
typedef int32_t rt_int32_t;
typedef uint32_t rt_uint32_t;


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
#define DATA_DIR_WRITE  (1 << 0)
#define DATA_DIR_READ   (1 << 1)
#define RESP_MASK   (0xF)
#define RESP_R1     (1 << 0)
#define RESP_R2     (3 << 0)

// see components/drivers/include/drivers/mmcsd_host.h
#define MMCSD_BUS_WIDTH_4       2


#define DELAY_MICROS delay_us
#define INIT_DEVICE_EXPORT(x)


#define rt_kprintf printf
#define mmcsd_dbg printf

void rt_free(void *rmem);
void *rt_memset(void *s, int c, size_t n);


