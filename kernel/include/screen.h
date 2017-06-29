#include "device/video.h"
void write_screen(int start, int color, int length);
void init_screen();
void screen_enter();
void screen_print_string(char str[], int color);
void draw_character(char ch, int x, int y, int color);
void draw_string(const char *str, int x, int y, int color);
