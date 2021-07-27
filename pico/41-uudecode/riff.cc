#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

FILE *fp;

//constexpr uint32_t chid(const char* id) { return *(uint32_t*)id; }

//#define CHID(x) *(uint32_t*) x

constexpr uint32_t chid(char c1, char c2, char c3, char c4) { return (c4 <<24) | (c3 << 16) | (c2 << 8) | c1; }


constexpr auto riff = chid('R', 'I', 'F', 'F');
constexpr auto wave = chid('W', 'A', 'V', 'E');
constexpr auto fmt_ = chid('f', 'm', 't', ' ');
//constexpr uint32_t data = chid("DATA");
constexpr auto data = chid('d', 'a', 't', 'a');
constexpr auto list = chid('L', 'I', 'S', 'T');

string unchid(uint32_t id)
{
	string res = "";
	for(int i = 0; i<4; i++) {
		res += (char) (id & 0xFF);
		id >>= 8;
	}
	return res;
}
struct hdr {
	uint32_t id;
	uint32_t size;
} hdr;

struct fmt_info {
	uint32_t len_fmt; // lendth of the format data (s/b 16)
	uint16_t type;
	uint16_t num_channels;
	uint32_t sample_rate; // samples per second
	uint32_t byte_rate; // SampleRate * BitsPerSample * Channels / 8
	uint16_t block_align; // BitsPerSample * Channels / 8 ; 1= 8bit mono, 2 = 8bit stereo or 16bit mono; 4 = 16bit stereo
} fmt_info;


bool is_even(int n) { return (n % 2 == 0); }
bool is_odd(int n) { return ! is_even(n); }

size_t file_pos = 0;

size_t file_size;

void read_hdr()
{
	fread(&hdr, 8, 1, fp);
	file_pos += 8;
}

void process_list(void)
{
	cout << "Processing list\n" ;
	char list_id[4];
	fread(list_id, 4, 1, fp);
	if(strncmp(list_id, "INFO", 4)) {
		cout << "Not an INFO, So skipping\n";
		fseek(fp, hdr.size -4, SEEK_CUR);
		return;
	}
	assert(is_even(hdr.size));
	size_t remaining = hdr.size-4;
	while(remaining > 0) {
		cout << "Remaining: " << remaining << "\n";
		read_hdr();
		cout <<  unchid(hdr.id) << ": " ;
		for(int i =0; i<hdr.size; i++) {
			char c;
			fread(&c, 1, 1, fp);
			if(c !=0) putchar(c);
		}
		cout << "\n";
		remaining = remaining - hdr.size - 8;
		if(is_odd(remaining)) {
			cout << "Decrementing due to odd boundary\n";
			char c;
                        fread(&c, 1, 1, fp);
			remaining--;
		}
	}
	return;
}

int main()
{
#if 0 
	char filename[] = "/home/pi/Music/sheep.wav";
#else
	char filename[] = "/home/pi/tmp/joined.wav";
#endif

	fp = fopen(filename, "r");
	assert(fp);

	read_hdr();
	assert(hdr.id == riff);
	file_size = hdr.size;

	read_hdr();
	assert(hdr.id == wave);
	assert(hdr.size == fmt_);
	size_t n = fread(&fmt_info, sizeof(struct fmt_info), 1, fp);
	file_pos += sizeof(struct fmt_info);
	assert(n==1);	
	cout << "len = " << fmt_info.len_fmt << "\n";
	assert(fmt_info.len_fmt == 16);
	cout << "type: " << fmt_info.type << "\n";
	cout << "#channs: " << fmt_info.num_channels << "\n";
	cout << "sample rate: " << fmt_info.sample_rate << "\n";

	while(ftell(fp) < file_size) {
	//for(int i = 0; i < 2; i++) {
		read_hdr();
		switch(hdr.id) {
			case list:
				process_list();
				break;
			case data:
				cout << "found data\n" ;
				fseek(fp, hdr.size, SEEK_CUR);
				break;
			default:
				cout << "unknown chunk id: " << unchid(hdr.id) << "\n";
				fseek(fp, hdr.size, SEEK_CUR);
		}
	}



	cout << "Bye\n";
	fclose(fp);


	return 0;
}
