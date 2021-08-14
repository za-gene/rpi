/* print a directory tree
*/

#include <stdio.h>

#include "fat32.h"

#define BDS_ATTR_RO  (1<<0) // read only
#define BDS_ATTR_HID (1<<1) // hidden
#define BDS_ATTR_SYS (1<<2) // operating syste,
#define BDS_ATTR_VID (1<<3) // volume id
#define BDS_ATTR_DIR (1<<4) // subdirectory
#define BDS_ATTR_ARC (1<<5) // archive: changed since last backup


void print_attrs(uint8_t attrs)
{
	//printf("--"); //unused

	putchar(attrs & BDS_ATTR_ARC ? 'A' : '-');
	putchar(attrs & BDS_ATTR_VID ? 'V' : '-');
	putchar(attrs & BDS_ATTR_SYS ? 'S' : '-');
	putchar(attrs & BDS_ATTR_HID ? 'H' : '-');
	putchar(attrs & BDS_ATTR_DIR ? 'D' : 'F');
	putchar(attrs & BDS_ATTR_RO ? 'R' : 'W');
}

void pindent(int indent)
{
	for(int i = 0; i<indent; i++) putchar(' ');
}

void dir_recurse(uint32_t cluster, int indent)
{
	bds_t bds;
	Dir dir(cluster);
	while(dir.read(bds)) {
		bool isdir = (bds.attr & BDS_ATTR_DIR);
		pindent(indent);
		printf("%-11.11s %8d %8d ", bds.name, bds.size, bds.fcl);
		print_attrs(bds.attr);
		puts("");
		if(bds.name[0] == '.') continue; // either current or parent dir
		if(isdir) {
			pindent(indent);
			printf("subdir begin: %s\n", bds.name);
			dir_recurse(bds.fcl, indent+3);
			pindent(indent);
			printf("subdir end: %s\n", bds.name);
		}

	}
}

int main()
{

	fat32_init();
	dir_recurse(0, 0);
	fat32_deinit();
	return 0;
}
