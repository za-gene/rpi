//#include <algorithm>
#include <assert.h>
#include <inttypes.h>
//#include <ctype.h>
//#include <cstring>
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

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
	static_assert(sizeof(volid) == 512);


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
	Dir dir;
	while(dir.read(bds)) 
		printf("%-11.11s %8d\n", bds.name, bds.size);
}

Dir::Dir()
{
	readablock(block_cluster(root_dir_first_cluster), (uint8_t*) bdss);
}

bool Dir::read(bds_t& bds) 
{
	while(1) {
		if(i==16) return false;
		bds = bdss[i++];
		uint8_t type = bds.name[0]; // first byte determines validity
		if(type == 0xE5) continue; // skip entry. It is an empty slot
		if(bds.attr & 0b10) continue; // skip hidden. LFNs are hidden
		if(type == 0) return false;
		return true;
	}
}
bool find(bds_t& bds, const std::string& outfile)
{
	Dir dir;
	while(dir.read(bds)) {
		if(strncmp(outfile.c_str(),  bds.name,11)==0)
			return true;
	}
	return false;
}


File::File(const char filename[12])
{
	//bds_t bds0;
	m_found = find(m_bds0, filename);
	if(!m_found) return;
	seek0();
	blocks_per_cluster = bytes_per_cluster/512;
}

// reset to beginning
void File::seek0(void)
{
	num_bytes_unread = m_bds0.size;
	cluster = m_bds0.fcl;
	blockn = 0;
}

bool File::found()
{
	return m_found;
}

int File::read(uint8_t block[512])
{
	if(!m_found) return 0;
	if(num_bytes_unread == 0) return 0;
	if(blockn == blocks_per_cluster) {
		//puts("-- READING CLUSTER");
		cluster = next_cluster(cluster);
		blockn = 0;
	}
	readablock(block_cluster(cluster) + blockn, block);
	blockn++;
	uint32_t bytes_read = std::min(num_bytes_unread, (uint32_t) 512);
	num_bytes_unread -=  bytes_read;
	return bytes_read;
}

/* convert a file to its FAT32 8.3 format
 */
std::string canfile(const std::string& infile)
{
	//std::string infile{argv[1]};
	std::size_t found = infile.find_first_of(".");
	std::string pre, post;
	if(found == std::string::npos) {
		pre = infile.substr(0, std::min((size_t)8, infile.size()));
		post = "   ";
	} else {
		pre = infile.substr(0, std::min((size_t)8, found));
		post = infile.substr(found+1, 3);
	}
	while(pre.size() <8) pre += ' ';
	while(post.size() <3) post += ' ';
	std::string outfile(pre+post);
	for(int i = 0; i< outfile.size(); i++) outfile[i] = toupper(outfile[i]);
	//std::transform(outfile.begin(), outfile.end(), outfile.begin(), toupper);
	assert(outfile.size() == 11);
	return outfile;
	//printf("pre     '%s', post='%s'\n", pre.c_str(), post.c_str());
	//printf("outfile '%s'\n", outfile.c_str());
}
