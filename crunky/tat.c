#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <tat.h>

u32 part_start = 0; // partition at which the block starts
u32 part_size = 0; // number of blocks in partition


int sd_readablock(uint block_num, uchar* buffer);

#ifndef max
#define max(a, b) ((a)>(b)? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a)<(b)? (a) : (b))
#endif


/** a convenience function
 */
static int streq(char* cmd, char* str) {
	return strcmp(cmd, str) == 0;
}


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


void tat_cat(char* path)
{
	int slot;
	tae_t* tae;
	for(slot=0; slot<NTAES; slot++) {
		tae = &tat.taes[slot];
		if((tae->flags & 1) && streq(tae->name, path)) break; 
	}

	if(slot == NTAES) {
		puts("File not found");
		return;
	}

	//lseek(part_fd, tae->start, SEEK_SET);
	char buffer[512];
	int size = tae->size;
	u32 offset_block = part_start + tae->start;
	printf("file size %d\n", size);
	for(;;) {
		if(size == 0) break;
		printf("size = %d\n", size);
		if(size <=0) break;
		sd_readablock(offset_block++, buffer);
		int nread = min(size, 512);
		//ssize_t nread = read(part_fd, buffer, min(size, sizeof(buffer)));
		write(0, buffer, nread);
		if(size<sizeof(buffer)) break;
		size -= sizeof(buffer);
	}

}
