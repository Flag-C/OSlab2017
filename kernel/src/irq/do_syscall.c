#include "x86/x86.h"
#include "common.h"
#include "assert.h"
#include "process.h"
#include "fs.h"

void serial_printc(char ch);
void write_screen(int start, int color, int length);
bool query_key(int index);
void draw_background(char background[]);
void draw_pixel(int x, int y, int color);
void write_palette();
void release_key(int index);
int get_time();
void prepare_buffer(void);
void display_buffer(void);
void sys_create_thread();

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
	case 4:
		draw_background((char *)tf->ebx);
		break;
	case 5:
		prepare_buffer();
		break;
	case 6:
		release_key(tf->ebx);
		break;
	case 7:
		tf->eax = get_tick();
		break;
	case 8:
		display_buffer();
		break;
	case 9:
		tf->eax = sys_fork();
	case 10:
		sys_sleep(tf->ebx);
		break;
	case 11:
		sys_exit(tf->ebx);
		break;
	case 12:
		tf->eax = sys_getpid();
		break;
	case 13: // sem_open(char *name, int val, int type)
		sem_init((char *)tf->ebx, tf->ecx, tf->edx);
		break;
	case 14: // sem_close(char *name)
		sem_destory((char *)tf->ebx);
		break;
	case 15: // sem_wait(char *name)
		sem_wait((char *)tf->ebx);
		break;
	case 16: // sem_post(char *name)
		sem_post((char *)tf->ebx);
		break;
	case 17:
		sys_create_thread(tf->ebx);
		break;
	case 18: // sys_fopen(const char *path, const char *mode)
		tf->eax = sys_fopen((char *)tf->ebx, (char *)tf->ecx);
		break;
	case 19: // sys_fread(void *dst, int size, int index)
		tf->eax = sys_fread((void *)tf->ebx, tf->ecx, tf->edx);
		break;
	case 20: // sys_fwrite(void *src, int size, int index)
		tf->eax = sys_fwrite((void *)tf->ebx, tf->ecx, tf->edx);
		break;
	case 21: // sys_fclose(int index)
		tf->eax =  sys_fclose(tf->ebx);
		break;
	case 4098: break;
	default:
		printk("Undefined system call: %d %d\n", tf->eax, tf->ebx);
		assert(0);
	}
}
