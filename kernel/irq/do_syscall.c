#include "x86/x86.h"
#include "common.h"
#include "assert.h"

void serial_printc(char ch);
void write_screen(int start, int color, int length);
bool query_key(int index);

void do_syscall(struct TrapFrame *tf)
{
	switch (tf->eax) {
	case 1:
		serial_printc(tf->ebx);
		break;
	case 2://draw_pixel(int x, int y, int color)
		draw_pixel(tf->ebx, tf->ecx, tf->edx);
		break;
	case 3:
		tf->eax = query_key(tf->ebx);
		break;
	default:
		printk("Undefined system call: %d %d", tf->eax, tf->ebx);
		assert(0);
	}
}
