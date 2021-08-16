#pragma once

int sdcard_init();
int readablock(unsigned int lba, unsigned char *buffer);
int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num);
