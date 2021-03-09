/*
 * I release this file into the Public Domain, together with
 * all the other files in this directory.
 *
 * Enjoy!
 *
 * Mark Carter, Jan 2021
 *
 */



#include <algorithm>
#include <assert.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string>
#include <iostream>
//#include <stdlib.h>

#ifdef PICO_BOARD
	#include "pico/stdlib.h"
	bool echo_char = true;
	extern void init_abba_pico();
	void init_extras() { init_abba_pico(); }
#else
	void stdio_init_all() {}
	bool echo_char = false;
	void init_extras() { }
#endif

#include <functional>
#include <vector>

#include "abba.h"

//#include "tokens.h"

/* EXCEPTIONS
 * 100 invalid_argument (string to number)
 */



enum tokens { PRINT = 257, ID };

enum opcodes { CALL = 1, LOAD};

using namespace std;


extern vector<prim_t> prims;

vector<u32> prog;


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


void eval_hi() { puts("hello world"); }




void p_halt()
{
	puts("p_halt called");
	while(1);
	exit(1);
}


u32 Call(fnptr prim)
{
	for(int i = 0; i<prims.size(); i++) {
		if(prims[i].fn == prim) {
			return (CALL<<24) + i;
		}
	}

	puts("Err: prim not found");
	return 666;
}

u32 Load(u8 reg, u32 value)
{
	return (LOAD<<24) + (reg << 16) + value;
}


void eval_twice()
{
	printf("Twice %d is %d\n", regs[0], 2*regs[0]);
}



int readchar()
{
	int c = getchar();
	if(echo_char) putchar(c);
	return c;
}


void push_bcode(u32 bcode) { prog.push_back(bcode); }



void parse_hi()
{
	prog.push_back(Call(eval_hi));
}

void parse_twice()
{
	yylex();
	prog.push_back(Load(0, xstoi(yytext)));
	prog.push_back(Call(eval_twice));
}


std::string yyupper;
int yylex()
{
	int c;

	yytext = "";
	while((c = readchar()) && isspace(c));
	yytext = c;
	while((c = readchar()) && !isspace(c)) { yytext += c; }
	//cout << "token:<" << yytext << ">\n";
	yyupper = toupper(yytext);
	return 1;
}



bool cmd_is(std::string text)
{
   return yyupper == text;
}

int xstoi(string str)
{
	try {
		return stoi(str);
	} catch (const invalid_argument& ex) {
		throw 100;
	}
}	

//auto prims = vector<prim_t> 
vector<prim_t> prims = 
{
	{"TWICE", parse_twice, eval_twice},
	{"HI", parse_hi, eval_hi}
};

std::string toupper(std::string str)
{
	string result = str;
	for(int i=0; i< result.size(); i++) result[i] = toupper(result[i]);
	return result;

}

void embed_num(u8 reg_num, u32 num)
{
	push_bcode(Load(reg_num, num));
}

void embed_num(u8 reg_num, std::string str)
{
	push_bcode(Load(reg_num, xstoi(str)));
}

// top level parsing
void parse_top()
{

	//loop:
	yylex();
	//string upper = toupper(yytext);
	for(const auto& prim: prims) {
		if(prim.name == yyupper) {
			prim.parse();
			return;
		}
	}
	throw 100;


	//goto loop;
}

void repl()
{
	printf("> ");
	parse_top();
	//puts("Finished lexing");

	int ip = 0;
	while(ip < prog.size()) {
		int ins = prog[ip++];
		u8 opcode = ins >> 24;
		u8 reg_num = (ins >> 16) & 0xFF;
		u32 opvalue = ins & 0xFFFF;
		switch(opcode) {
			case CALL : {
					    //int fn_idx = prog[opvalue];
					    auto& fn = prims[opvalue].fn;
					    fn();
					    break;
				    }
			case LOAD : {
					    regs[reg_num] = opvalue;
					    break;
				    }

			default:
				    throw 16;
		}
	}
}

int main ()
{
	stdio_init_all();
	//getchar();
	init_extras();
	puts("abba: a basic basic. type run to execute");

	prog.reserve(10000);	
	while(1) {
		try { 
			prog.clear();
			repl();
		} catch(int ex) {
			printf("Error: exception number %d\n", ex);
		}
	}
	puts("Bye");
	return 0;
}

