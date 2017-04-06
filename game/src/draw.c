#include "game.h"
#include "string.h"
#include "resources.h"
extern char font8x8_basic[128][8];
/* 绘制屏幕上的内容。
 * 注意程序在绘图之前调用了prepare_buffer，结束前调用了display_buffer。
 * prepare_buffer会准备一个空白的绘图缓冲区，display_buffer则会将缓冲区绘制到屏幕上，
 * draw_pixel或draw_string绘制的内容将保存在缓冲区内(暂时不会显示在屏幕上)，调用
 * display_buffer后才会显示。
*/
void
draw_pixel_u(int x, int y, int color)
{
	asm volatile("int $0x80"::"a"(2), "b"(x), "c"(y), "d"(color));
	return;
}
void
draw_bg_u(unsigned char background[])
{
	asm volatile("int $0X80"::"a"(4), "b"(background));
	return;
}

void
draw_character(char ch, int x, int y, int color)
{
	int i, j;
	assert((ch & 0x80) == 0);
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

void
prepare_buffer_u(void)
{
	asm volatile("int $0X80"::"a"(5));
	return;
}

void
display_buffer_u(void)
{
	asm volatile("int $0X80"::"a"(8));
	return;
}

void
redraw_screen()
{
	fly_t it;
	const char *hit, *miss;

	prepare_buffer_u(); /* 准备缓冲区 */

	int i, j;
	draw_bg_u(background);


	/* 绘制每个字符 */
	for (it = characters(); it != NULL; it = it->_next) {
		static char* buf;
		buf = "1s\0";
		draw_string(buf, it->x, it->y, it->color);
	}


	for (i = 0; i < 8; i ++)
		for (j = 0; j < table_length; j ++)
			if (accel) draw_pixel_u(SCR_HEIGHT - 16 + i, table_location + j, 200);
			else draw_pixel_u(SCR_HEIGHT - 16 + i, table_location + j, 14);
	draw_string(itoa(get_time()), SCR_HEIGHT - 8 - 8, 16, 48);
	hit = itoa(get_hit());
	draw_string("He:+", 8, SCR_WIDTH - strlen(hit) * 8 - 48, 225);
	draw_string(hit, 8, SCR_WIDTH - strlen(hit) * 8 - 16, 225);
	draw_string("s", 8, SCR_WIDTH - 16, 225);
	miss = itoa(get_miss());
	draw_string("You:-", SCR_HEIGHT - 16, SCR_WIDTH - strlen(miss) * 8 - 56, 130);
	draw_string(miss, SCR_HEIGHT - 16, SCR_WIDTH - strlen(miss) * 8 - 16, 130);
	draw_string("s", SCR_HEIGHT - 16, SCR_WIDTH - 16, 130);
	draw_string(itoa(get_fps()), 8, 8, 255);
	draw_string("FPS", 8, strlen(itoa(get_fps())) * 8 + 8, 255);
	//printk("%d", table_location);

	display_buffer_u(); /* 绘制缓冲区 */
}
