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
//#include <bitfield>
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
#include "tusb.h"
bool echo_char = true;



void init_abba()
{ 
	extern void init_abba_pico();
	stdio_init_all();  
	while(!tud_cdc_connected()) sleep_ms(250);
	init_abba_pico();
}

#else
void init_abba() {}
bool echo_char = false;
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

vector<bcode_t> prog;


////////////////////////////////////////////////////////////////////////////






void p_halt()
{
	puts("p_halt called");
	while(1);
	exit(1);
}


bcode_t Call(fnptr prim)
{
	for(int i = 0; i<prims.size(); i++) {
		if(prims[i].fn == prim) {
			return {CALL, i};
		}
	}

	throw 666;
	//puts("Err: prim not found");
	//return 666;
}

bcode_t Load(u8 reg, i32 value)
{
	return {LOAD,  (reg << 16) + value};
}





int readchar()
{
	int c = getchar();
	if(echo_char) putchar(c);
	return c;
}


void push_bcode(bcode_t bcode) { prog.push_back(bcode); }



void eval_hi() { 
	puts("hello world"); 
}
void parse_hi()
{
	prog.push_back(Call(eval_hi));
}

void eval_twice()
{
	printf("Twice %d is %d\n", regs[0], 2*regs[0]);
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
begin:
	while((c = readchar()) && isspace(c));

	if(c == '#') { // eat comments
		do {
			c = readchar();
		} while (c != '\r' && c != '\n');
		goto begin;
	}

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
		bcode_t ins = prog[ip++];
		u8 opcode = ins.code;
		u8 reg_num = (ins.operand >> 16) & 0xFF;
		u32 opvalue = ins.operand & 0xFFFF;
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
	init_abba();
	puts("abba: a basic basic. type run to execute. 'hi' prints a welcoming message");

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

