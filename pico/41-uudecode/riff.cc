#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
//#include <fstream>
#include <unistd.h>

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
typedef struct __attribute__((__packed__)) {
	uint32_t id; // RIFF, WAVE, etc.
	uint32_t size;
	uint32_t begin; // file position where the chunk data begins
	uint32_t end; // file position where the chunk data ends
} hdr_t;

typedef struct {
	uint32_t len_fmt; // length of the format data (s/b 16)
	uint16_t type;
	uint16_t num_channels;
	uint32_t sample_rate; // samples per second
	uint32_t byte_rate; // SampleRate * BitsPerSample * Channels / 8
	uint16_t block_align; // BitsPerSample * Channels / 8 ; 1= 8bit mono, 2 = 8bit stereo or 16bit mono; 4 = 16bit stereo
	uint16_t bits_per_sample;
} wave_fmt_t;


bool is_even(int n) { return (n % 2 == 0); }
bool is_odd(int n) { return ! is_even(n); }

/* align file to an even offset */
bool align (FILE* fp)
{
	if(is_even(ftell(fp))) return false;
	cout << "aligning fileptr\n";
	fseek(fp, 1, SEEK_CUR);
	return true;
}

void ahead (FILE* fp, int offset)
{ 
	fseek(fp, offset, SEEK_CUR); 
	align(fp);
}

size_t file_pos = 0;

//uint32_t file_size;

void read_hdr (hdr_t* hdr)
{
	uint32_t n = fread(hdr, 1, 8, fp);
	assert(n==8);
	hdr->begin = ftell(fp);
	hdr->end = hdr->begin + hdr->size;
	//file_pos += 8;
}

void dump_list (hdr_t* hdr)
{
	cout << "LIST chunk...\n" ;
	char list_id[4];
	fread(list_id, 4, 1, fp);
	if(strncmp(list_id, "INFO", 4)) {
		cout << "Not an INFO, So skipping\n";
		fseek(fp, hdr->size -4, SEEK_CUR);
		return;
	}
	assert(is_even(hdr->size));
	size_t remaining = hdr->size-4;
	while(remaining > 0) {
		cout << "Remaining: " << remaining << "\n";
		read_hdr(hdr);
		cout <<  unchid(hdr->id) << ": " ;
		for(int i =0; i<hdr->size; i++) {
			char c;
			fread(&c, 1, 1, fp);
			if(c !=0) putchar(c);
		}
		cout << "\n";
		remaining = remaining - hdr->size - 8;
		if(align(fp)) remaining--;
	}
	return;
}


void dump_wave_fmt (FILE* fp)
{

	cout << "FMT chunk...\n";
	wave_fmt_t fmt;
	fseek(fp, -4, SEEK_CUR);
	fread(&fmt, 1, sizeof(wave_fmt_t), fp);
	//uint32_t chunk_size += sizeof(struct fmt_info);
	cout << "len = " << fmt.len_fmt << "\n";
	assert(fmt.len_fmt == 16);
	cout << "type: " << fmt.type << "\n";
	cout << "#channs: " << fmt.num_channels << "\n";
	cout << "sample rate: " << fmt.sample_rate << "\n";
}

void dump_wave (hdr_t* hdr, FILE* fp)
{
	cout << "\nWAVE chunk...\n";

	// wind back the file a little, because "size" is actually a chunk name
	fseek(fp, -4, SEEK_CUR);

	while(1) {
		hdr_t hdr1;
		read_hdr(&hdr1);
		switch(hdr1.id) {
			case list:
				dump_list(&hdr1);
				break;
			case fmt_:
				dump_wave_fmt(fp);
				//ahead(fp, hdr1.size);
				break;
			case data:
				cout << "found data\n";
				ahead(fp, hdr1.size);
				return;
			default:
				cout << "Unknown wave type;" << unchid(hdr1.id) << ". Aborting.\n";
				exit(1);
		}
	}
				
}

void dump_file(const char* filename)
{
	cout << "RIFF process\n";
	fp = fopen(filename, "r");
	assert(fp);
	cout << "intial ftell = " << ftell(fp) << "\n";
	hdr_t hdr;
	read_hdr(&hdr);
	assert(hdr.id == riff);
	cout << "header ends " << hdr.end << "\n";
	//uint32_t end = hdr.size+8;

	while(ftell(fp) < hdr.end) {
		hdr_t hdr1;
		read_hdr(&hdr1);
		//cout << "chunk size = " << hdr1.size << "\n";
		//cout << "chunk ends = " << hdr1.end << "\n";
		//uint32_t end = hdr.size;
		switch(hdr1.id) {
			case wave: 
				dump_wave(&hdr1, fp); 				
				//ahead(fp, hdr1.size);
				cout << "ftell now at " << ftell(fp) << "\n";
				break;
			default:
				cout << "RIFF doesn't understand " << unchid(hdr1.id) << ", skipping\n";
				ahead(fp, hdr1.size);
		}
	}

	cout << "Bye\n";
}

void print_help(void)
{
	const char* text = R"EOI(
riff - edit RIFF files
a [FILE] append file to end 
h	print this help
)EOI";
	puts(text);
	exit(0);
}

void append_file (const char* filename, const char* append_filename)
{
#if 0
	fp = fopen(filename, "r+"); // for reading and writing
	assert(fp);
	read_hdr();
	assert(hdr.id == riff);
	uint32_t chunk_size = hdr.size;
	//size_t fsize = file
	
	//ifstream fin(fielanme);
	FILE* fapp = fopen(append_filename, "r");
	assert(fapp);
	fread(&hdr, 8, 1, fapp);
	//read_hdr();
	assert(hdr.id == riff);

	// check that we have a WAVE chunk
        fread(&hdr, 8, 1, fapp);
	assert(hdr.id == wave);

	//cout << "fapp seek = " << ftell(fapp);
	fseek(fapp, -8, SEEK_CUR); // now wind back to start of WAVE chunk
	//cout << ", and now " << ftell(fapp) << "\n";

	//append WAVE in fapp to fp
	fseek(fp, 0, SEEK_END);
	char block[512];
	size_t n, cum = 0;
	while(1) {
		n = fread(block, 1, sizeof(block), fapp);
		if(n<=0) break;
		//cout << "appending " << n << "\n";
		fwrite(block, 1, n,  fp);
		cum += n;
	}

	// update the length of RIFF
	file_size += cum;
	fseek(fp, 4, SEEK_SET); // size of file is at offset 
	fwrite(&file_size, 4, 1, fp);	


	fclose(fapp);
#endif
}

int main (int argc, char** argv)
{
	int option;
	char *append_filename = nullptr;
	while((option = getopt(argc, argv, "a:h")) != -1) {
		switch(option) {
			case 'a': append_filename = optarg; break;
			case 'h': print_help();
			case '?': printf("unknown option: %c\n", optopt); exit(1);
		}
	}

	/*
	   for(; optind < argc; optind++){ //when some extra arguments are passed
	   printf("Given extra arguments: %s\n", argv[optind]);
	   }
	   */

	cout << "optind=" << optind << ", argc = " << argc << "\n";
	if(argc <= optind) {
		puts("Filename unspecified. Aborting");
		exit(1);
	}

	char *filename = argv[optind];

	if(append_filename) {
		append_file(filename, append_filename);
	} else {
		dump_file(filename);
	}

	fclose(fp);


	return 0;
}


