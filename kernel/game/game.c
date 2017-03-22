#include "common.h"
#include "irq.h"
#include "x86/x86.h"
#include "device/video.h"
#include "device/timer.h"
#include "device/font.h"
#include "device/palette.h"
#include "game.h"

#define FPS 30
#define CHARACTER_PER_SECOND 2
#define UPDATE_PER_SECOND 100

volatile int tick = 0;

void
timer_event(void)
{
	tick ++;
}

int
get_time(void)
{
	return tick / HZ;
}

static int real_fps;
void
set_fps(int value)
{
	real_fps = value;
}
int
get_fps()
{
	return real_fps;
}


void game_main_loop()
{
	int now = 0, target;
	int num_draw = 0;
	table_location = 2;
	bool redraw;
	accel = FALSE;
	while (TRUE) {
		wait_for_interrupt();
		disable_interrupt();
		if (now == tick) {
			enable_interrupt();
			continue;
		}
		assert(now < tick);
		target = tick; /* now总是小于tick，因此我们需要“追赶”当前的时间 */
		enable_interrupt();

		redraw = FALSE;
		while (update_keypress())
			;

		/* 依次模拟已经错过的时钟中断。一次主循环如果执行时间长，期间可能到来多次时钟中断，
		 * 从而主循环中维护的时钟可能与实际时钟相差较多。为了维持游戏的正常运行，必须补上
		 * 期间错过的每一帧游戏逻辑。 */
		while (now < target) {
			/* 每隔一定时间产生一个新的字符 */
			if (now % (HZ / CHARACTER_PER_SECOND) == 0)
				create_new_letter();
			/* 每隔一定时间更新屏幕上字符的位置 */
			if (now % (HZ / UPDATE_PER_SECOND) == 0)
				update_letter_pos();
			/* 每隔一定时间需要刷新屏幕。注意到这里实现了“跳帧”的机制：假设
			 *   HZ = 1000, FPS = 100, now = 10, target = 1000
			 * 即我们要模拟990个时钟中断之间发生的事件，其中包含了9次屏幕更新，
			 * 但redraw flag只被置一次。 */
			if (now % (HZ / FPS) == 0)
				redraw = TRUE;
			/* 更新fps统计信息 */
			if (now % (HZ / 2) == 0) {
				int now_fps = num_draw * 2 + 1;
				if (now_fps > FPS) now_fps = FPS;
				set_fps(now_fps);
				num_draw = 0;
			}
			now ++;
		}

		if (redraw) { /* 当需要重新绘图时重绘 */
			num_draw ++;
			redraw_screen();
		}
	}
}