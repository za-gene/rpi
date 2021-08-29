#include <stdlib.h>
#include <string.h>

#include "drv_sdio_shim.h"

void rt_free(void *rmem)
{
	free(rmem);
}

void *rt_memset(void *s, int c, size_t n)
{
	return memset(s, c, n);
}

rt_uint32_t __rt_fls(rt_uint32_t val)
{
	rt_uint32_t  bit = 32;

	if (!val)
		return 0;
	if (!(val & 0xffff0000u))
	{
		val <<= 16;
		bit -= 16;
	}
	if (!(val & 0xff000000u))
	{
		val <<= 8;
		bit -= 8;
	}
	if (!(val & 0xf0000000u))
	{
		val <<= 4;
		bit -= 4;
	}
	if (!(val & 0xc0000000u))
	{
		val <<= 2;
		bit -= 2;
	}
	if (!(val & 0x80000000u))
	{
		bit -= 1;
	}

	return bit;
}

void mmcsd_req_complete(struct rt_mmcsd_host *host)
{
	//rt_sem_release(&host->sem_ack);
}
