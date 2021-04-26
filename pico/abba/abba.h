#pragma once

#include <stdint.h>

#include <string>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t i32;

//using fn_t = std::function<void(void)>;
typedef void (*fnptr)();
typedef struct {const char* name; fnptr parse; fnptr fn; } prim_t;


typedef struct {
	unsigned char code : 8;
	int operand : 24;
} bcode_t;

void push_bcode(bcode_t bcode);
bcode_t Call(fnptr prim);
bcode_t Load(u8 reg, i32 value);
int yylex();
int xstoi(std::string str);
inline i32 regs[15];
inline std::string yytext;
//inline std::string yy;
void embed_num(u8 reg_num, std::string str);
void embed_num(u8 reg_num, u32 num);
bool cmd_is(std::string text);
std::string toupper(std::string str);
