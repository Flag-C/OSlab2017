#include "game.h"
#include "common.h"
#include "string.h"
#include "adt/linklist.h"
#include "device/video.h"
#include "x86/x86.h"

LINKLIST_IMPL(fly, 10000)

static fly_t head = NULL;
static int hit = 0, miss = 0;

int
get_hit(void)
{
	return hit;
}

int
get_miss(void)
{
	return miss;
}

fly_t
characters(void)
{
	return head;
}

/* 在屏幕上创建一个新的字母 */
void
create_new_letter(void)
{
	if (head == NULL) {
		head = fly_new(); /* 当前没有任何字母，创建新链表 */
	} else {
		fly_t now = fly_new();
		fly_insert(NULL, head, now); /* 插入到链表的头部 */
		head = now;
	}
	/* 字母、初始位置、掉落速度均为随机设定 */
	head->x = 0;
	head->y = rand() % (SCR_WIDTH / 8 - 2) * 8 + 8;
	head->v = (rand() % 1000 / 1000.0 + 1) / 1.5;
	head->text = rand() % 26;
	release_key(head->text); /* 清除过往的按键 */
}

/* 逻辑时钟前进1单位 */
void
update_letter_pos(void)
{
	fly_t it;
	for (it = head; it != NULL; ) {
		fly_t next = it->_next;
		it->x += it->v; /* 根据速度更新位置 */
		if (it->x + 7.9 > SCR_HEIGHT - 16 &&
		    it->x > table_location &&
		    it->x < table_location + table_length) {
			hit ++;
			fly_remove(it);
			fly_free(it);
			if (it == head) head = next;

		} else if (it->x + 7.9 > SCR_HEIGHT ) {
			miss++;
			fly_remove(it);
			fly_free(it);
			if (it == head) head = next;
		}

		it = next;
	}
}


/* 更新按键 */
bool
update_keypress(void)
{
	fly_t it, target = NULL;

	disable_interrupt();
	if (query_key(1)) {
		release_key(1);
		if (table_location > 2) table_location -= 16;
	} else if (query_key(0)) {
		release_key(0);
		if (table_location + table_length < SCR_WIDTH - 16) table_location += 16;
	}

	enable_interrupt();

	return FALSE;
}
