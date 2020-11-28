#pragma once

#include <stdio.h>

typedef int (*fn_putchar)(int);

void set_putchar(fn_putchar fn);
void newline();

//int putchar(int c);
//int puts(const char *s);

