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

void push_bcode(u32 bcode);
u32 Call(fnptr prim);
u32 Load(u8 reg, u32 value);
int yylex();
int xstoi(std::string str);
inline i32 regs[15];
inline std::string yytext;
