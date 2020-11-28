#include <yastdio.h>


// an implementation that does nothing
int _putchar_nada(int c)
{
	return EOF;
}

fn_putchar _putchar = _putchar_nada;

void set_putchar(fn_putchar fn)
{
	_putchar = fn;
}

int putchar(int c)
{
	return _putchar(c);
}

void newline()
{
	putchar('\n');
}

int puts(const char *s)
{
	if(!s) return EOF;
	while(*s) {
		int ok = putchar(*s++);
		if(ok == EOF) return EOF;
	}
	return putchar('\n');
}

