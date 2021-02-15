#pragma once

#include <stdint.h>

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


void tat_ls();
void tat_mount();
void tat_cat(char* path);
int tat_open(char* path);
int tat_size();
int tat_read(char* buffer);

