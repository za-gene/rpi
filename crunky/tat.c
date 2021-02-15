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
	//printf("ID: %x\n", pinfo.id);
	u32 start_block = pinfo.rel/256;
	part_start = start_block;
	//printf("Start block (of size 512): %d\n", start_block);
	u32 num_blocks = pinfo.nsecs/256;
	part_size = num_blocks;
	//printf("Size in blocks: %d\n", num_blocks);
	sd_readablock(part_start, (uchar*) &tat);
	return;
}


// ugly ugly hacks. You can only open one file at a time
tae_t* g_cur_tae;
u32 g_seek;
//u32 g_block;

int tat_open(char* path)
{
	g_seek = 0;
	//g_block = g_cur_tae->start;
	int slot;
	//tae_t* tae;
	for(slot=0; slot<NTAES; slot++) {
		g_cur_tae = &tat.taes[slot];
		if((g_cur_tae->flags & 1) && streq(g_cur_tae->name, path)) return slot+2; 
	}
	return -1;
}

int tat_size() { return g_cur_tae->size; }

int tat_read(char* buffer)
{
	int nread = min(512, g_cur_tae->size - g_seek);
	if(nread==0) return 0;
	//if(g_seek >= g_cur_tae->size
	sd_readablock(g_seek/512 + g_cur_tae->start + part_start, buffer);
	g_seek += nread;
	return nread;
}

void tat_cat(char* path)
{
	int fd  = tat_open(path);

	if(fd == -1) {
		puts("File not found");
		return;
	}

	char buffer[512];
	//int eof;
	int nread;
	while(nread = tat_read(buffer)) {
		write(1, buffer, nread);
	}
}
