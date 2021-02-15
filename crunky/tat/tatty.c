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

#include <tat.h>


int part_fd; // a file descriptor for the partition

extern u32 part_start; // partition at which the block starts
extern u32 part_size; // number of blocks in partition

static int streq(char* cmd, char* str) {
	return strcmp(cmd, str) == 0;
}

int sd_readblock(uint lba, uchar* buffer, uint num)
{
	//puts("sd_block called");
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



int main(int argc, char* argv[])
{

	if(argc<2) {
		puts("insuffient arguments");
		return 1;
	}

	//printf("Size of allocation table is %d\n", sizeof(tat));


#if 1
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

	if(streq(cmd, "ls")) tat_ls();
	if(streq(cmd, "store")) tat_store(argv[2], argv[3]);
	if(streq(cmd, "cat")) tat_cat(argv[2]);
	if(streq(cmd, "mount")) tat_mount();

	close(part_fd);

	return 0;
}
