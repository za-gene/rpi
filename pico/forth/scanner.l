%{
#include "tokens.h"
%}

delim	[ \t\r\n]
ws	{delim}+
letter	[a-z]|[A-Z]
id	{letter}+

%option noyywrap

%%

{ws}	{}
print	{ puts("FOUND a print statement"); return PRINT; }
{id}	{ printf("Found id: <%s>\n", yytext); return ID; }

%%

