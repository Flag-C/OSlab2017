#include "common.h"
#include "printf.h"
char *libitoa(int a)
{
	static char buf[30];
	char *p = buf + sizeof(buf) - 1;
	do {
		*--p = '0' + a % 10;
	} while (a /= 10);
	return p;
}

int
libstrlen(const char *s)
{
	int n;

	for (n = 0; *s != '\0'; s++)
		n++;
	return n;
}

int
libstrnlen(const char *s, size_t size)
{
	int n;

	for (n = 0; size > 0 && *s != '\0'; s++, size--)
		n++;
	return n;
}