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


// an implementation that does nothing
int _getchar_nada()
{
	return EOF;
}


fn_getchar _getchar = _getchar_nada;

void set_getchar(fn_getchar fn)
{
	_getchar = fn;
}

int getchar()
{
	return _getchar();
}



int newline()
{
	return putchar('\n');
}

int print_string(const char *s)
{
	if(!s) return EOF;
	while(*s) {
		int ok = putchar(*s++);
		if(ok == EOF) return EOF;
	}
	return 1;
}

int puts(const char *s)
{
	print_string(s);
	newline();
	return 1;
}

