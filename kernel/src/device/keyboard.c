#include "common.h"
#include "string.h"
#define RIGHT 0x4d
#define LEFT 0x4b
#define SHIFT_PRESS 0x2a
#define SHIFT_REALSE 0xaa

static int letter_code[] = {
	RIGHT, LEFT, SHIFT_PRESS, SHIFT_REALSE,
};
/* 对应键按下的标志位 */
static bool letter_pressed[4];

void
press_key(int scan_code)
{
	int i;
	for (i = 0; i < 4; i ++) {
		if (letter_code[i] == scan_code)
			letter_pressed[i] = TRUE;
	}
}

void
release_key(int index)
{
	assert(0 <= index && index < 4);
	letter_pressed[index] = FALSE;
}

bool
query_key(int index)
{
	assert(0 <= index && index < 4);
	return letter_pressed[index];
}

/* key_code保存了上一次键盘事件中的扫描码 */
static volatile int key_code = 0;

int last_key_code(void)
{
	return key_code;
}

void
keyboard_event(int code)
{
	key_code = code;
	press_key(code);
}
