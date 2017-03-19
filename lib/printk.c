#include "common.h"

/* implement this function to support printk */
void get_num(char* arr, int base, int n)
{
	int sig;
	sig = 0;
	if (n == 0) {
		*arr = '0';
		*(arr + 1) = '\0';
		return;
	}

	if (base == 10 && n < 0) {
		sig = 1;
		n = -n;
	}

	uint32_t num;
	num = (uint32_t)n;
	char start[30];
	char *temp = start;
	if (num == 4294967295 && base == 10) {
		arr = "-2147483648\0";
		return;
	}
	while (num != 0) {
		int mod = num % base;
		if (mod < 10)
			*temp = '0' + mod;
		else
			*temp = 'a' + mod - 10;
		num = num / base;
		temp++;
	}
	temp--;
	if (sig == 1) {
		*arr = '-';
		arr++;
	}
	while (temp != start - 1) {
		*arr = *temp;
		arr++;
		temp--;
	}

	*arr = '\0';
}

void vfprintf(void (*printer)(char), const char *ctl, void **args)
{
	char c;
	for (c = *ctl; *ctl != '\0'; ctl++, c = *ctl)
		if ( c == '%' && *(ctl + 1) != '\0') {
			ctl++;
			char arr[100];
			if ( *ctl == 'd') {
				get_num(arr, 10, *(int*)args);
				char *ch;
				for (ch = arr; *ch != '\0'; ch++)
					printer(*ch);
				args++;
			} else if (*ctl == 'x') {
				get_num(arr, 16, *(int*)args);
				char *ch;
				for (ch = arr; *ch != '\0'; ch++)
					printer(*ch);
				args++;
			} else if (*ctl == 's') {
				char *p = *(char **)args;
				while (*p != '\0') {
					printer(*p);
					p++;
				}
				args++;
			} else if (*ctl == 'c') {
				printer(*(char*)args);
				args++;
			} else {
				printer('%');
				args++;
			}
		} else
			printer(c);
}

extern void serial_printc(char);


/* __attribute__((__noinline__))  here is to disable inlining for this function to avoid some optimization problems for gcc 4.7 */

void __attribute__((__noinline__))

printk(const char *ctl, ...)
{
	void **args = (void **)&ctl + 1;
	vfprintf(serial_printc, ctl, args);
}