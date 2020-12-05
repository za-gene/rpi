#pragma once

#include <stdio.h>

typedef int (*fn_putchar)(int);
typedef int (*fn_getchar)();

void set_getchar(fn_putchar fn);
void set_putchar(fn_putchar fn);
int newline();
int print_string(const char *s);

//int putchar(int c);
//int puts(const char *s);

