/* CRC8 calculation
 *
 * 2021-08-11 mcarter created
 *
 * See
 * https://stackoverflow.com/questions/29214301/ios-how-to-calculate-crc-8-dallas-maxim-of-nsdata
 *
 * and
 * https://www.leonardomiliani.com/en/2013/un-semplice-crc8-per-arduino/
 *
 * Some CRC7 implementations:
 * https://github.com/hazelnusse/crc7/blob/master/crc7.cc
 * https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/lib/crc7.c?h=v4.15-rc2
 */

#include "crc8.h"


unsigned char crc8_dallas_chunk(unsigned char crc, const unsigned char * data, const unsigned int size)
{
	for ( unsigned int i = 0; i < size; ++i )
	{
		unsigned char inbyte = data[i];
		for ( unsigned char j = 0; j < 8; ++j )
		{
			unsigned char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if ( mix ) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

unsigned char crc8_dallas(const unsigned char * data, const unsigned int size)
{
	unsigned char crc = 0;
	return crc8_dallas_chunk(crc, data, size);
}
