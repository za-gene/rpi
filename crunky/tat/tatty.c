/* format the filesystem
*/
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef unsigned char uchar;
typedef unsigned int uint;


typedef struct __attribute__((__packed__)) {
	u32 start; // start block relative to partition
	u32 res;   // reserved blocks for file
	u32 size; // actual size used in bytes
	u8  flags;
	char name[10];
} tae_t; // tiny allocation entry


#define NTAES 20
struct  __attribute__((__packed__)) {
	char magic[8];
	tae_t taes[NTAES];  // tiny allocation table
	char padding[44]; // pad tat to exactly 512 bytes
} tat;

int part_fd; // a file descriptor for the partition

u32 part_start = 0; // partition at which the block starts
u32 part_size = 0; // number of blocks in partition

int streq(char* cmd, char* str) {
	return strcmp(cmd, str) == 0;
}

int sd_readblock(uint lba, uchar* buffer, uint num)
{
	puts("sd_block called");
	assert(sizeof(off_t)>= sizeof(u32));
	u32 offset = (u32)lba * 512;
	lseek(part_fd, offset, SEEK_SET);
	read(part_fd, buffer, num*512);
	return num*512;

}

int sd_readablock(uint block_num, uchar* buffer)
{
	return sd_readblock(block_num, buffer, 1);
}


int sd_writeablock(uchar* buffer, u32 block_num)
{
	u32 offset = (u32) block_num * 512;
	lseek(part_fd, offset, SEEK_SET);
	return write(part_fd, buffer, 512);
}

void tat_flush_table()
{
	sd_writeablock((uchar*) &tat, part_start); // table goes into first block of partition
}

void tat_store_tat()
{
	lseek(part_fd, 0, SEEK_SET);
	write(part_fd, (void*) &tat, sizeof(tat));
}

void tat_fmt()
{
	assert(part_start);

	u32 reserve = (part_size -1)/NTAES; // the blocks we will reserve for each file


/*
	struct stat statbuf;
	fstat(part_fd, &statbuf);
	u32 size = statbuf.st_size;
	printf("Total size:%d\n", size);

	u32 offset = sizeof(tat);
	u32 file_size = (size -offset)/NTAES;
	printf("Each file is of size %d\n", file_size);
*/
	strncpy(tat.magic, "TATFS01.", sizeof(tat.magic)); // no null-termination
	for(int i=0; i<NTAES; i++) {
		tae_t* tae = &tat.taes[i];
		tae->start = i*reserve + 1;
		tae->res = reserve;
		tae->size = 0;
		tae->flags = 0;
		strcpy(tae->name, "EMPTY");
	}
	tat_flush_table();

}

void tat_ls()
{
	printf("%2s %7s %7s %7s %7s %s\n", "#", "Start", "Alloc", "Size", "Flags", "Name");
	for(int i=0; i<NTAES; i++) {
		tae_t* tae = &tat.taes[i];
		printf("%2d %7d %7d %7d %7d %s\n", i, tae->start, tae->res, tae->size, tae->flags, tae->name);
	}
}


void tat_store(char* real, char* as)
{
	int slot;
	tae_t* tae;
	for(slot=0; slot<NTAES; slot++) {
		tae = &tat.taes[slot];
		if(tae->flags==0) break;
	}

	if(slot == NTAES) {
		puts("File system full");
		return;
	}

	printf("slot %d, as %s\n", slot, as);
	strcpy(tae->name, as);

	FILE* fp = fopen(real, "r");
	assert(fp);

	char buffer[512];

	//int offset = tae->start;
	//lseek(part_fd, offset, SEEK_SET);
	int nread;
	int size =0;
	u32 offset_block = part_start + tae->start;
	while(nread = fread(buffer, 1, sizeof(buffer), fp)) {
		printf(",");
		sd_writeablock(buffer, offset_block++); 
		//write(part_fd, buffer, nread);
		size += nread;
	}
	//tat_flush();
	fclose(fp);
	tae->size = size;
	tae->flags = 1;
	tat_flush_table();


	//tat_ls();


}

#ifndef max
#define max(a, b) ((a)>(b)? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a)<(b)? (a) : (b))
#endif

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
	sd_readblock(0, buffer, 1);
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

int main(int argc, char* argv[])
{

	if(argc<2) {
		puts("insuffient arguments");
		return 1;
	}

	printf("Size of alloaction table is %d\n", sizeof(tat));


#if 0
	char *dev = "tat.fs";
#else
	char *dev = "/dev/loop0";
#endif
	part_fd = open(dev, O_RDWR);
	assert(part_fd != 1);

	assert(sizeof(tat) == 512);

	tat_mount();

	char* cmd = argv[1];
	if(streq(cmd, "fmt")) {
		puts("found format");
		tat_fmt();
	} 
	/*
	else {
		// load tat
		lseek(part_fd, 0, SEEK_SET);
		read(part_fd, &tat, sizeof(tat));
	}
	*/

	if(streq(cmd, "ls")) tat_ls();

	if(streq(cmd, "store")) tat_store(argv[2], argv[3]);

	if(streq(cmd, "cat")) tat_cat(argv[2]);

	if(streq(cmd, "mount")) tat_mount();
	//tat_store_tat();

	close(part_fd);

	return 0;
}
