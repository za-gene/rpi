#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "fat32.h"

int main(int argc, char** argv)
{
	if(argc != 2) {
		puts("Err: Requires a filename to be specified. Aborting.");
		exit(1);
	}

	fat32_init();
	file32_type(argv[1]);
	fat32_deinit();
	return 0;
}
