/*
 * I release this file into the Public Domain, together with
 * all the other files in this directory.
 *
 * Enjoy!
 *
 * Mark Carter, Jan 2021
 *
 */



#include <assert.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include "pico/stdlib.h"
#include <functional>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

using fn_t = std::function<void(void)>;



////////////////////////////////////////////////////////////////////////////
// dictionary begin

typedef struct {
	int prev; // point to a position on the heap
	u8 flags;
} word_t;


int latest = -1;


// dictionary end
////////////////////////////////////////////////////////////////////////////
// heap functions begin

u8 heap[10000];
u32 htop = 0;

void push_heap_byte(u8 b) { heap[htop++] = b; }

void push_heap_u32(u32 val) 
{ 
	for(int i=0; i<4; i++) {
		heap[htop++] = val & 0xff;
		val >>= 8;
       	}
}


void push_heap(u8* bytes, int len)
{
	while(len--) push_heap_byte(*bytes++);
}


u32 get32(int pos)
{
	u32 val = 0;
	for(int i=0; i<4; i++) {
		val <<= 8;
		val += heap[pos+i];
       	}
	return val;
}

// heap functions end
////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////

static char word_pad[64]; // scratch counted buffer to hold word found

void word_pad_cstr(const char* zstr)
{
	u8 len = strlen(zstr);
	memcpy(word_pad+1, zstr, len);
	*word_pad = len;
}

void create_full_header(u8 flags, const char* cstr, u32 fn)
{
	u8 len = *cstr;
	push_heap((u8*) cstr, len+1);
	word_t word;
	word.prev = latest;
	word.flags = flags | len;
	latest = htop;
	push_heap((u8*) &word, sizeof(word_t));
	push_heap_u32(fn);
}


void p_hi() { puts("hello world"); }

void p_words()
{
	int words = latest;
	while(words >=0) {
		word_t* word = (word_t*) (*heap+word);
		int len = heap[words] & ~(1<<7);
#if 1
		printf("\nword len=%d\n", len);
		for(int i = 0; i<len; i++) putchar( heap[words-len+i]); 
		putchar('\n');
#endif
		words = get32(words+1);
		printf("next word: %d\n", words);
		//words = word->prev;

	}
}

typedef struct {u8 flags; const char* zname; fn_t fn; } prim_t;
prim_t prims[] =  {
	{0,	"WORDS", p_words},
	{0,	"HI",	p_hi},

	{0,0,0}
};

void add_primitives()
{
	prim_t* p = prims;
	int primn = 0;
	while((p->zname)) {
		word_pad_cstr(p->zname);
		create_full_header(p->flags, word_pad,  (1<<30) & primn++);
		p++;
	}
}


void dump_heap()
{
	FILE* fp = fopen("heap.bin", "w");
	fwrite(heap, sizeof(heap), 1, fp);
	fclose(fp);
}

int main ()
{
	add_primitives();
	dump_heap();
	p_words();

	puts("Bye");
	return 0;
}

