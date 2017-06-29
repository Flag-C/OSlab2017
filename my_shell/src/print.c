#include "string.h"
#include "../include/lib.h"

static int position_x, position_y;

void init_screen()
{
	position_x = 1;
	position_y = 1;
}

void shell_enter()
{
	position_x ++;
	position_y = 1;
}

void shell_print_string(char str[], int color)
{
	int len = strlen(str);
	if (str[0] == '\n') {
		shell_enter();
		printf("\n");
		return;
	}
	if (position_y + len > 39)
		shell_enter();
	draw_string(str, position_x << 3, position_y << 3, color);
	printf("%s", str);
	position_y += len;
}
