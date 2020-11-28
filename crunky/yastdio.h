#pragma once

#include <stdio.h>

typedef int (*fn_putchar)(int);

void set_putchar(fn_putchar fn);
int newline();
int print_string(const char *s);

//int putchar(int c);
//int puts(const char *s);

