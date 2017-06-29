#include "string.h"
#include "screen.h"

int printk(const char *, ...);

static int position_x, position_y;
void
write_screen(int start, int color, int length)
{
	memset((void *)vmem + start, color, length);
}


void screen_init()
{
	position_x = 1;
	position_y = 1;
}

void screen_roll_up(int x)
{
	int i;
	for (i = 0; i < SCR_HEIGHT - x; i ++)
		memcpy((void *)vmem + i * SCR_WIDTH, (void *)vmem + (i + x)*SCR_WIDTH, SCR_WIDTH);
	memset((void *)vmem + i * SCR_WIDTH, 0, x * SCR_WIDTH);
}

void screen_enter()
{
	position_x ++;
	position_y = 1;
	if (position_x > 23) {
		screen_roll_up(8);
		position_x = 23;
	}
}

void screen_print_string(char str[], int color)
{
	int i, len = strlen(str);
	for (i = 0; i < len; i ++) {
		if (str[i] == '\n')
			screen_enter();
		else if (str[i] == '\t') {
			position_y = ((position_y - 2) / 6 + 1) * 6 + 1;
			if (position_y >= 39)
				screen_enter();
		} else {
			draw_character(str[i], position_x << 3, position_y << 3, color);
			if (position_y ++ >= 38)
				screen_enter();
		}
	}
	printk("%s", str);
}

extern char font8x8_basic[128][8];

void
draw_character(char ch, int x, int y, int color)
{
	int i, j;
	char *p = font8x8_basic[(int)ch];
	for (i = 0; i < 8; i ++)
		for (j = 0; j < 8; j ++)
			if ((p[i] >> j) & 1)
				write_screen((x + i) * SCR_WIDTH + y + j, color, 1);
}

void
draw_string(const char *str, int x, int y, int color)
{
	while (*str) {
		draw_character(*str ++, x, y, color);
		if (y + 8 >= SCR_WIDTH) {
			x += 8; y = 0;
		} else {
			y += 8;
		}
	}
}
