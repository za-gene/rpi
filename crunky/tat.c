#include <string.h>
#include <stdio.h>

#include <tat.h>

u32 part_start = 0; // partition at which the block starts
u32 part_size = 0; // number of blocks in partition


int sd_readablock(uint block_num, uchar* buffer);

void tat_ls()
{
	printf("%2s %7s %7s %7s %7s %s\n", "#", "Start", "Alloc", "Size", "Flags", "Name");
	for(int i=0; i<NTAES; i++) {
		tae_t* tae = &tat.taes[i];
		printf("%2d %7d %7d %7d %7d %s\n", i, tae->start, tae->res, tae->size, tae->flags, tae->name);
	}
}



void tat_mount()
{
	struct __attribute__((__packed__)) {
		u8 flag;
		u8 head;
		u16 dummy1; // actually starting sector/cyl
		u8 id;
		u16 dummy2; // actually ending sec/cyl
		u32 rel;
		u32 nsecs; // total sectors in partition
	} pinfo;

	char buffer[512];
	sd_readablock(0, buffer);
	memcpy(&pinfo, buffer+0x01CE, sizeof(pinfo));
	printf("ID: %x\n", pinfo.id);
	u32 start_block = pinfo.rel/256;
	part_start = start_block;
	printf("Start block (of size 512): %d\n", start_block);
	u32 num_blocks = pinfo.nsecs/256;
	part_size = num_blocks;
	printf("Size in blocks: %d\n", num_blocks);

	sd_readablock(part_start, (uchar*) &tat);

	// dump first sector of partition
	//sd_readblock(pinfo.rel*2, buffer, 1);
	//assert(pinfo.rel*2 == 314572800UL);
	return;
	sd_readablock(start_block, buffer);
	for(int i=0; i <50; i++) {
		//printf("%d ", (int) buffer[i]);
		putchar(buffer[i]);
	}
	//write(1, buffer, 512);
}
