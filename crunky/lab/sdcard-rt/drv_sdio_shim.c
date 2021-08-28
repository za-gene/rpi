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
