#include <ctype.h>


int toupper(int c)
{
	if(islower(c))
		return c - 32;
	else
		return c;
}

int islower(int c)
{
	return (('a' <= c ) && (c <= 'z'));
}


int isupper(int c)
{
	return (('A' <= c ) && (c <= 'Z'));
}

