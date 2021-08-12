#include <inttypes.h>
#include <stdio.h>

int main()
{
	/*
	struct S {
		char b1 :6;
		char b2 :6;
		char b3 :6;
		char b4 :6;
	} s;
	*/

	struct S1 {
		auto b : 128;
	} s;

	// packing:
	// in  00000000 11111111 22222222
	// out 00000011 11112222 22333333
	unsigned char uublock[3];
	//uublock[3] = 0;
	FILE *fp = fopen("lorem.txt", "r");
	puts("begin 644 lorem");
	for(int j = 0; j<15; j++) {
		//if(n < 3) break;
		auto n = fread(uublock, 3, 1, fp);

		unsigned char b = (uublock[0] >> 2) +32; // out 0
		printf("%c", b);
		b = ((uublock[0] & 0b11) << 4) + (uublock[1] >>4) + 32; // out 1
		printf("%c", b);
		b = ((uublock[1] & 0b1111) << 2) + (uublock[2] >>6) + 32; // out2
		printf("%c", b);
		b = (uublock[2] & 0b111111) + 32; // out 3
		printf("%c", b);
	}

	return 0;
}
