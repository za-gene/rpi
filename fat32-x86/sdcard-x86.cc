// an abstraction layer for accessing the device

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

static FILE* fp = nullptr;

int readablock (int blocknum, uint8_t block[512])
{
	fseek(fp, blocknum*512, SEEK_SET);
	fread(block, 512, 1, fp);
	return 0;
}


void sdcard_init(void)
{
	fp = fopen("/dev/loop0", "r");
	assert(fp);
}

void sdcard_deinit(void)
{
	fclose(fp);
	fp=0;
}

