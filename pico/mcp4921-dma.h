#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void mcp4921_dma_init(void);
void mcp4921_dma_put(uint16_t vol);


#ifdef __cplusplus
}
#endif

