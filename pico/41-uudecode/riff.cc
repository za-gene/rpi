#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

FILE *fp;

struct hdr {
	char id[4];
	uint32_t size;
} hdr;

void read_hdr()
{
	fread(&hdr, 16, 1, fp);
}

int main()
{
	fp = fopen("/home/pi/Music/sheep.wav", "r");
	assert(fp);
	read_hdr();
	fclose(fp);


	return 0;
}
