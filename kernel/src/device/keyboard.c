#include "common.h"
#include "string.h"
#define RIGHT 0x4d
#define LEFT 0x4b
#define SHIFT_PRESS 0x2a
#define SHIFT_REALSE 0xaa

static int letter_code[] = {
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44,  2,  3,  4,  5,
	6,  7,  8,  9, 10, 11, 52, 12, 57, 28, 54
};
/* 对应键按下的标志位 */
static bool letter_pressed[41];

void
press_key(int scan_code)
{
	int i;
	for (i = 0; i < 41; i ++) {
		if (letter_code[i] == scan_code)
			letter_pressed[i] = TRUE;
	}
}

void
release_key(int scan_code)
{
	int i;
	for (i = 0; i < 41; i ++) {
		if (letter_code[i] + 0x80 == scan_code) {
			letter_pressed[i] = false;
		}
	}
}


bool
query_key(int index)
{
	if (index >= 0x1000)
		index -= 0x1000;
	assert(0 <= index && index < 41);
	return letter_pressed[index];
}

int see_if_any_key_pressed()
{
	int i;
	for (i = 0; i < 40; i ++) {
		if (letter_pressed[i]) {
			if (letter_pressed[40]) {
				return i + 0x1000;
			} else
				return i;
		}

	}
	return -1;
}

/* key_code保存了上一次键盘事件中的扫描码 */
static volatile int key_code = 0;

int last_key_code(void)
{
	return key_code;
}

void
keyboard_event(int key_code)
{
	if (key_code < 0x80)
		press_key(key_code);
	else
		release_key(key_code);
}
