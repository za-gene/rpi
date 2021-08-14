#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "fat32.h"

/*extern int readablock (int blocknum, uint8_t block[512]);
extern uint32_t bytes_per_cluster;
extern uint32_t block_cluster(uint32_t cluster_number);
*/


int main(int argc, char** argv)
{
	if(argc != 2) {
		puts("Err: Requires a filename to be specified. Aborting.");
		exit(1);
	}

	//char outfile[12];
	fat32_init();
	char outfile[12];
	canfile(outfile, argv[1]);
	File file(outfile);
	if(!file.found()) puts("ERR: file not found");
	uint8_t block[512];
	while(int n = file.read(block)) {
		for(int i = 0; i< n; i++) putchar(block[i]);
	}


	fat32_deinit();
	return 0;
}
