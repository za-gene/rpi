#include <stdio.h>

extern void serial_println(const char* str);
extern int atl_eval(char *);

void serial_newline(void)
{
	putchar('\n');
}

void serial_print(const char* str)
{
	printf(str);
}

int serial_available(void) { return 1; }

int serial_read(void) { 
	char c = getchar();
	putchar(c);
	return c;
}

char t[132];
int readln()
{
	t[0] = 0;
	int n = 0;
	for (;;) {
		if(serial_available() > 0) {
			int b = serial_read();
			if(b == '\r') {serial_newline(); b = '\n'; }
			t[n++] = b;
			if (b == '\n') goto eoi;
		}
	}
eoi:
	t[n] = 0;
	return n - 1;
}
int kernel_main()
{
	while(1) {
		if(readln()) atl_eval(t);
		serial_println("\n ok");
	}

	return 0;
}
