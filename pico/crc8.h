#pragma once


unsigned char crc8_dallas(const unsigned char * data, const unsigned int size);
unsigned char crc8_dallas_chunk(unsigned char crc, const unsigned char * data, const unsigned int size);
