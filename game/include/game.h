#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "adt/linklist.h"
#include "printf.h"

#define HZ 1000
#define SCR_WIDTH  320
#define SCR_HEIGHT 200
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
/* 定义fly_t链表 */
LINKLIST_DEF(fly)
float x, y;
int color;
float v;
LINKLIST_DEF_FI(fly)


#define table_length 56
int table_location;
bool accel;

/* 主循环 */
void game_main_loop(void);

/* 游戏逻辑相关 */
void create_new_letter(void);
void update_letter_pos(void);
bool update_keypress(void);

int get_hit(void);
int get_time(void);
int get_miss(void);
int get_fps(void);
void set_fps(int fps);
fly_t characters(void);

void redraw_screen(void);

/* 随机数 */
int rand(void);
void srand(int seed);

#endif
