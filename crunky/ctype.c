#include <ctype.h>

int isspace(int c)
{
	if((c == '\f') || (c == '\n') || (c == '\r') || (c == '\t') || (c == '\v'))
		return 1;
	else
		return 0;
}

int isdigit(int c)
{
	return (('0' <=c) && (c<='9'));
}


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

