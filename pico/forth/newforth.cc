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
#include <iostream>
//#include <stdlib.h>
#ifdef PICO_BOARD
	#include "pico/stdlib.h"
#else
	void stdio_init_all() {}
#endif

#include <functional>
#include <vector>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t i32;

using namespace std;

//using fn_t = std::function<void(void)>;
typedef void (*fnptr)();


extern int yylex (void);


////////////////////////////////////////////////////////////////////////////
// dictionary begin

typedef struct {
	u8 flags;
	int prev; // point to a position on the heap
} word_t;


i32 latest = 1<<30;


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


i32 get32(int pos)
{
	i32 val = 0;
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
	printf("create_full_header:latest:%d\n", latest);
}


void p_hi() { puts("hello world"); }



//typedef function<void()> prim_t;

void p_words()
{
	puts("p_words1 called");
}

void p_halt()
{
	puts("p_halt called");
	while(1);
	exit(1);
}

//typedef fn_t* fnptr;
typedef struct {const char* name; fnptr fn; } prim_t;

auto prims = vector<prim_t> {
	{"WORDS", p_words},
	{"HALT", p_halt},
	{"HI", p_hi}
};

u32 Call(fnptr prim)
{
	for(int i = 0; i<prims.size(); i++) {
		if(prims[i].fn == prim) {
			return i;
		}
	}

	puts("Err: prim not found");
	return 666;
}


void eval(u32* ins)
{
	int ip = 0;
	//int fn_idx;

	while(1) {
		int fn_idx    	= ins[ip++];
		auto& fn = prims[fn_idx].fn;
		fn();
	}
}

int main ()
{
	stdio_init_all();
	//getchar();
	puts("abba: a basic basic");

	/*
	string line;
	getline(cin, line);
	cout << "Input line was " << line << endl;

	for(auto& c : line) {
		if(isspace(c)) continue;
		while(!
	}
	*/

	while(yylex());
	puts("Finished lexing");

	u32 prog[] = {Call(p_hi), Call(p_words), Call(p_halt)};
	eval(prog);

	puts("Bye");
	return 0;
}

