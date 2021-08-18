#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fat32.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

uint32_t fat_begin_lba, bytes_per_cluster, cluster_begin_lba, sectors_per_cluster, root_dir_first_cluster;

extern int readablock (int blocknum, uint8_t block[512]);
extern void sdcard_init(void);
extern void sdcard_deinit(void);

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// partition 

ptable_t part_tab; // partition table
//static_assert(sizeof(part_tab) == 4 *sizeof(pte_t));

typedef struct { u8 id; const char* desc; } part_names_t;


part_names_t part_names[] = {
	{0x00, "Empty"},
	{0x0b, "W95 FAT32"},
	{0x0c, "W95 FAT32 (LBA)"},
	{0x82, "Linux swap"}, 
	{0x83, "Linux"},
	{0x93, "Amoeba"},
	{0x63, "GNU Hurd or SysV"}
};

const char* find_part_name(u8 id)
{
	static char unk[] = "Unknown";
	for(int i=0; i< sizeof(part_names)/sizeof(part_names_t); i++) {
		if(part_names[i].id == id) return part_names[i].desc;
	}
	return unk;
}


void fat32_type_partition_table()
{
	// a sector is 512b long
	printf("\nParition table\n");
	printf("%-17s %8s %8s %8s %4s %s\n", 
			"Device     Boot", "Start", "End",  "Sectors",  "Id",  "Type");

	for(int part = 0; part<4; part++) {
		pte_t pte = part_tab[part];
		if(pte.rel_sect == 0) break;
		printf("%-12d ", part); // device number
		printf("%c    ", pte.boot ? '*' : ' '); // bootable?
		printf("%8d ", pte.rel_sect); // start sector
		printf("%8d ", pte.rel_sect + pte.tot_sect -1); // end sector
		printf("%8d ", pte.tot_sect); // number of sectors in partition
		printf("0x%.2x ", pte.sid); // id
		printf("%s", find_part_name(pte.sid)); // partition name
		printf("\n");

	}
}

// partition end
////////////////////////////////////////////////////////////////////////////

/* 0xFFFFFFFF indicates last cluster
 */

uint32_t next_cluster (uint32_t cluster)
{
	// RFE cache the fat_block to mitigate unecessary re-reading
	//printf("CLUSTER=%d\n", cluster);
	uint8_t fat_block[512];	
	//printf("CLUSTER=%d\n", cluster);
	uint32_t base_block = fat_begin_lba + (cluster*4)/512;
	uint32_t offset = (cluster*sizeof(uint32_t))%512;
	readablock(base_block, fat_block);
	//printf("base = %d, off = %d\n", base_block, offset);
	uint32_t new_cluster = *(uint32_t*) (fat_block + offset);
	//printf("next_cluster: old = %d, new = %d", cluster, new_cluster);
	return new_cluster;
}

// given a cluster, return its block number
uint32_t block_cluster(uint32_t cluster_number) 
{ 
		return cluster_begin_lba + (cluster_number - 2) * sectors_per_cluster;
}
	

void fat32_init (void)
{
	sdcard_init();
	u8 block[512];
	int status;
	status =readablock(0, block);	
	assert(status == 0);
	//ptable_t part_tab;
	memcpy(part_tab, block + 0x1BE, sizeof(ptable_t));
	//dump_partition(part_tab);
	assert(part_tab[0].sid == 0x0b); // need first partition to be W95 FAT32
	//puts("Partition layout as expected");


	// read the volume id
	uint32_t partition_lba_begin = part_tab[0].rel_sect;
	// volume id
	struct __attribute__((__packed__)) volid_t {
		char jmp[3]; // jump code+nop, which we don't use
		char oem[8]; 
		//char dummy[11];
		u16 bps; // bytes per sector. always 512
		u8 spc; //sector per cluster
		u16 nors; // number of reserved sectors. usually 0x20
		u8 nofs; // number of fats. Always 2
		char stuff1[19]; // likely irrelevant
		u32 spf; // sectors per fat
		char stuff2[4]; // bunch of other stuff that's likely to be irrelevant
		u32 rdfc; // root directory first cluster. usual 0x2
		char stuff3[462]; // bunch of other stuff that's likely to be irrelevant
		u16 sig; // signature. Always 0xAA55
	} volid;
	//static_assert(sizeof(volid) == 512);


	status = readablock(partition_lba_begin , block);
	memcpy(&volid, block, sizeof(volid));
	//printf("bytes per sector: %d\n", (int) volid.bps);
	//printf("size of volid %d\n", sizeof(volid));
	assert(volid.bps == 512);
	assert(volid.nofs == 2);
	assert(volid.sig == 0xAA55);

	// set global vars
	fat_begin_lba = partition_lba_begin + volid.nors;
	cluster_begin_lba = fat_begin_lba + volid.nofs * volid.spf;
	sectors_per_cluster = volid.spc;

	//printf("sectors per cluster: %d\n", sectors_per_cluster);
	bytes_per_cluster = sectors_per_cluster * volid.bps;
	//printf("bytes_per_cluster=%d\n", bytes_per_cluster);
	root_dir_first_cluster =  volid.rdfc;
	//cout << "root_dir_first_cluster = " << root_dir_first_cluster << "\n";
}

void fat32_deinit (void)
{
	sdcard_deinit();
}


void fat32_list_root (void)
{
	puts("Directory contents:");
	bds_t bds;
	dir32_t dir;
	dir32_init_root(&dir);
	//Dir dir;
	while(dir32_read(&dir, &bds)) 
		printf("%-11.11s %8d\n", bds.name, bds.size);
}

/*
void Dir::init_cluster(uint32_t dir_cluster)
{
	m_fat_cluster = block_cluster(dir_cluster);
	readablock(m_fat_cluster, (uint8_t*) bdss);
}

Dir::Dir(uint32_t dir_cluster)
{
	if(dir_cluster==0)
		dir_cluster = root_dir_first_cluster;
	init_cluster(dir_cluster);
}

Dir::Dir()
{
	init_cluster(root_dir_first_cluster);
}
*/

void dir32_init_cluster (dir32_t* dir, uint32_t dir_cluster)
{
	if(dir_cluster==0) dir_cluster = root_dir_first_cluster;
	dir->sector_block_num = 0;
	dir->i = 0;
	dir->m_fat_cluster = block_cluster(dir_cluster);
	readablock(dir->m_fat_cluster, (uint8_t*) dir->bdss);
}
void dir32_init_root (dir32_t* dir)
{
	dir32_init_cluster(dir, root_dir_first_cluster);
}

bool dir32_read(dir32_t* dir, bds_t* bds) 
{
	while(1) {
		/* there's something about bits 7-31 of the current cluster tells
		 * you which sectors to read from the FAT, and bits 0-6 tell you which
		 * of the 128 integers in that sector is the number of the next cluster
		 * of the file (or if all ones, that the current cluster is the last).
		 * Not really sure what all that means, so I'm ignoring as at 2021-08-10
		 */
		if(dir->i==16)  {
			// next sector
			dir->sector_block_num++;
			if(dir->sector_block_num == sectors_per_cluster) return false; // TODO should read another cluster
			readablock(dir->m_fat_cluster + dir->sector_block_num, (uint8_t*) dir->bdss);
			dir->i = 0;
		}
		*bds = dir->bdss[dir->i++];
		uint8_t type = bds->name[0]; // first byte determines validity
		if(type == 0xE5) continue; // skip entry. It is an empty slot
		if(bds->attr & 0b10) continue; // skip hidden. LFNs are hidden
		if(type == 0) return false;
		return true;
	}
}

#if 0
bool Dir::read(bds_t& bds) 
{
	while(1) {
		/* there's something about bits 7-31 of the current cluster tells
		 * you which sectors to read from the FAT, and bits 0-6 tell you which
		 * of the 128 integers in that sector is the number of the next cluster
		 * of the file (or if all ones, that the current cluster is the last).
		 * Not really sure what all that means, so I'm ignoring as at 2021-08-10
		 */
		if(i==16)  {
			// next sector
			sector_block_num++;
			if(sector_block_num == sectors_per_cluster) return false; // TODO should read another cluster
			readablock(m_fat_cluster + sector_block_num, (uint8_t*) bdss);
			i = 0;
		}
		bds = bdss[i++];
		uint8_t type = bds.name[0]; // first byte determines validity
		if(type == 0xE5) continue; // skip entry. It is an empty slot
		if(bds.attr & 0b10) continue; // skip hidden. LFNs are hidden
		if(type == 0) return false;
		return true;
	}
}
#endif

bool dir32_find(bds_t* bds, const char* canfile)
{
	dir32_t dir;
	dir32_init_root(&dir);
	//Dir dir;
	while(dir32_read(&dir, bds)) {
		if(strncmp(canfile,  bds->name,11)==0)
			return true;
	}
	return false;
}

/*
bool find(bds_t& bds, const char* outfile)
{
	Dir dir;
	while(dir.read(bds)) {
		if(strncmp(outfile,  bds.name,11)==0)
			return true;
	}
	return false;
}
*/

void file32_init (file32_t* file, const char canfile[12])
{
	file->num_bytes_unread = 0;
	file->blockn = 0;
	file->m_found = dir32_find(&(file->m_bds0), canfile);
	if(!file->m_found) return;
	file32_seek0(file);
	file->blocks_per_cluster = bytes_per_cluster/512;
}

/*
File::File(const char filename[12])
{
	//bds_t bds0;
	m_found = find(m_bds0, filename);
	if(!m_found) return;
	seek0();
	blocks_per_cluster = bytes_per_cluster/512;
}
*/

// reset to beginning
void file32_seek0(file32_t* file)
{
	file->num_bytes_unread = file->m_bds0.size;
	file->cluster = file->m_bds0.fcl;
	file->blockn = 0;
}

uint32_t file32_size(file32_t* file)
{
	return file->m_bds0.size;
}

/*
// reset to beginning
void File::seek0(void)
{
	num_bytes_unread = m_bds0.size;
	cluster = m_bds0.fcl;
	blockn = 0;
}
*/

bool file32_found(file32_t* file)
{
	return file->m_found;
}

/*
bool File::found()
{
	return m_found;
}
*/

int file32_read(file32_t* file, uint8_t block[512])
{
	if(!file->m_found) return 0;
	if(file->num_bytes_unread == 0) return 0;
	if(file->blockn == file->blocks_per_cluster) {
		//puts("-- READING CLUSTER");
		file->cluster = next_cluster(file->cluster);
		file->blockn = 0;
	}
	readablock(block_cluster(file->cluster) + file->blockn, block);
	file->blockn++;
	#define min(a,b) (((a) < (b)) ? (a) : (b))
	uint32_t bytes_read = min(file->num_bytes_unread, (uint32_t) 512);
	file->num_bytes_unread -=  bytes_read;
	return bytes_read;
}

void file32_type(const char* rawfilename)
{
	char outfile[12];
	canfile(outfile, rawfilename);
	file32_t file;
	file32_init(&file, outfile);
	//File file(outfile);
	if(!file32_found(&file)) puts("ERR: file not found");
	uint8_t block[512];
	int n;
	while(n = file32_read(&file, block)) {
		for(int i = 0; i< n; i++) putchar(block[i]);
	}
}



/*
// C seems to complain about regular toupper()
char toupper_a(char c)
{
	if(('a' <= c) && (c <= 'z')) return c -32;
	return c;
}
*/


/* convert a file to its FAT32 8.3 format
 */

void canfile(char dst[12], const char* src)
{
	int i;
	for(i=0; i<12; i++) dst[i]=' ';
	dst[11] = 0;

	for(i=0; i< 8; i++) {
		if(src[i] == 0) return;
		if(src[i] == '.') break;
		dst[i] = toupper(src[i]);
	}

	if(src[i]==0) return;

	if(src[i] == '.') {
		i++;
	} else {
		while(src[i] != '.' ) {
			if(src[i] == 0) return;
			i++;
		}
		i++;
	}

	for(int j = 0; j<3; j++) {
		char c= toupper(src[j+i]);
		if(c==0) return;
		dst[j+8] = c;
	}

}


bool file32_is_canonical(const char *filename)
{
        if(strlen(filename) != 11) return false;

        while(*filename) {
                char c = *filename;
                if(!(isupper(c) || c == ' ')) return false;;
                filename++;
        }
        return true;
}

bool fats32_initialised = false;

void fat32_soft_init(void)
{
        if(fats32_initialised) return;
        fats32_initialised = true;
        fat32_init();
}

uint8_t* file32_slurp(const char *filename, uint32_t *len, bool *found)
{
        *len = 0;
        *found = false;
        fat32_soft_init();
        file32_t file;
        char cooked_name[12];
        if(file32_is_canonical(filename))
                strcpy(cooked_name, filename);
        else
                canfile(cooked_name, filename);
        file32_init(&file, cooked_name);
        *found = file32_found(&file);
        if(!*found) return 0;
        *len = file32_size(&file);
        char* data = malloc(*len);
        size_t offset = 0;
        while(file32_read(&file, data + offset)) offset += 512;
        return data;
}

