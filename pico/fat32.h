#pragma once

#include <inttypes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif



// partition table entry
typedef struct __attribute__((__packed__))  {
	uint8_t boot;
	uint8_t start_head;
	uint16_t start_alt;
	uint8_t sid;
	uint8_t end_head;
	uint16_t end_alt;
	uint32_t rel_sect;
	uint32_t tot_sect;
} pte_t;

typedef pte_t ptable_t[4];
static_assert(sizeof(ptable_t)  == 4*sizeof(pte_t));


typedef struct __attribute__((__packed__))  bds_t {
	char name[11];
	uint8_t attr;
	char unused1[8];
	uint16_t fch; // first cluster high
	char unused2[4];
	uint16_t fcl; // first cluster low
	uint32_t size; // file saize
} bds_t; // byte directory structure
static_assert(sizeof(bds_t) == 32);


typedef struct {
	bds_t bdss[16]; // total size is 512
	int sector_block_num;
	int i;
	uint32_t m_fat_cluster;
} dir32_t;
//static_assert(sizeof(dir32_t.bdss) == 512);

void dir32_init_cluster(dir32_t* dir, uint32_t dir_cluster);
void dir32_init_root(dir32_t* dir);
bool dir32_read(dir32_t* dir, bds_t* bds);
bool dir32_find(bds_t* bds, const char* canfile);

/*
class Dir
{
	public:
		Dir(); 
		Dir(uint32_t dir_cluster);
		bool read(bds_t& bds);
	private:
		bds_t bdss[16];
		void init_cluster(uint32_t dir_cluster);
		static_assert(sizeof(bdss) == 512);
		int sector_block_num = 0;
		int i = 0;
		uint32_t m_fat_cluster;
};
*/

typedef struct {
	bool m_found;
	uint32_t num_bytes_unread;
	uint32_t cluster;
	uint32_t blocks_per_cluster;
	uint32_t blockn;
	bds_t m_bds0; // info about the start of the file
} file32_t;

void file32_init(file32_t* file, const char canfile[12]);
void file32_seek0(file32_t* file);
bool file32_found(file32_t* file);
int file32_read(file32_t* file, uint8_t block[512]);
void file32_type(const char* rawfilename);


/*
class File {
	public:
		File(const char filename[12]);
		int read(uint8_t sector_block[512]);
		bool found();
		void seek0(void);
		uint32_t size(void);
	private:
		bool m_found = false;
		uint32_t num_bytes_unread = 0;
		uint32_t cluster, blocks_per_cluster;
		uint32_t blockn = 0;
		bds_t m_bds0; // info about the start of the file
};
*/

void fat32_init(void);
void fat32_type_partition_table(void);
void fat32_deinit(void);
void fat32_list_root(void);
uint32_t type_cluster(uint32_t cluster, uint32_t max_num_bytes);
uint32_t next_cluster (uint32_t cluster);
void canfile(char dst[12], const char* src);

#ifdef __cplusplus
}
#endif

