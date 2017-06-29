#include "types.h"
#include "common.h"
#include "../include/lib.h"

extern char font8x8_basic[128][8];
void show_rootdir(bool ls_l, bool ls_a, char res[512])
{
	asm volatile("int $0x80" : : "a"(24), "b"(ls_l), "c"(ls_a), "d"(res));
}

int file_open(const char *path, const char *mode)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(18), "b"(path), "c"(mode));
	return ret;
}

int file_read(void *dst, int size, int index)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(19), "b"(dst), "c"(size), "d"(index));
	return ret;
}

int file_write(void *src, int size, int index)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(20), "b"(src), "c"(size), "d"(index));
	return ret;
}

int file_seek(int index, int offset, int whence)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(22), "b"(index), "c"(offset), "d"(whence));
	return ret;
}

int file_close(int index)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(21), "b"(index));
	return ret;
}

bool query_key_u(int index)
{
	int ret;
	asm volatile("int $0x80":"=a"(ret):"a"(3), "b"(index));
	return ret;
}

int see_if_any_key_pressed()
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(23));
	return ret;
}

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

void serial_printc_u(char p)
{
	asm volatile("int $0x80"::"a"(1), "b"(p));
	return;
}


/* __attribute__((__noinline__))  here is to disable inlining for this function to avoid some optimization problems for gcc 4.7 */

void __attribute__((__noinline__))
printf(const char *ctl, ...)
{
	void **args = (void **)&ctl + 1;
	vfprintf(serial_printc_u, ctl, args);
}

void
draw_pixel_u(int x, int y, int color)
{
	asm volatile("int $0x80"::"a"(2), "b"(x), "c"(y), "d"(color));
	return;
}

void
draw_character(char ch, int x, int y, int color)
{
	int i, j;
	char *p = font8x8_basic[(int)ch];
	for (i = 0; i < 8; i ++)
		for (j = 0; j < 8; j ++)
			if ((p[i] >> j) & 1)
				draw_pixel_u(x + i, y + j, color);
}

void
draw_string(const char *str, int x, int y, int color)
{
	while (*str) {
		draw_character(*str ++, x, y, color);
		if (y + 8 >= SCR_WIDTH) {
			x += 8; y = 0;
		} else
			y += 8;
	}
}