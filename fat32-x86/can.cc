/* test out the conversion of files to their canonical form
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "fat32.h"



void check(const char* file, int line, const char* x, const char* y)
{
	char dst[12];
	canfile(dst, x);
	const char* pass = strcmp(dst, y) ? "ERR" : "OK";
	printf("%s: '%s' vs '%s' for '%s'\n", pass, dst, y, x);
}


#define CHECK(x, y) check(__FILE__, __LINE__, x, y)
//#define CHECK(x, y) assert(strcmp(x, y) == 0)

int main()
{
	CHECK("foo.", 		"FOO        ");
	CHECK("foo.x", 		"FOO     X  ");
	CHECK("foo", 		"FOO        ");
	CHECK("readme.txt", 	"README  TXT");
	CHECK("go.a.b", 	"GO      A.B");
	CHECK("go.a.", 		"GO      A. ");
	return 0;
}
