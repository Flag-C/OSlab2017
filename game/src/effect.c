#include "game.h"
#include "common.h"
#include "string.h"
#include "adt/linklist.h"
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

void release_key_u(int index)
{
	asm volatile("int $0x80"::"a"(6), "b"(index));
	return;
}

bool query_key_u(int index)
{
	int ret;
	asm volatile("int $0x80":"=a"(ret):"a"(3), "b"(index));
	return ret;
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
	head->v = (rand() % 1000 / 1000.0 + 1) / 2;
	head->color = rand() % 225;
}

/* 逻辑时钟前进1单位 */
void
update_letter_pos(void)
{
	fly_t it;
	for (it = head; it != NULL; ) {
		fly_t next = it->_next;
		it->x += it->v; /* 根据速度更新位置 */
		if (it->x + 7.9 + 16 > SCR_HEIGHT &&
		    it->y > table_location &&
		    it->y < table_location + table_length) {
			hit ++;
			fly_remove(it);
			fly_free(it);
			if (it == head) head = next;

		} else if (it->x + 7.9 > SCR_HEIGHT ) {
			if (it->y > table_location &&
			    it->y < table_location + table_length)
				hit++;
			else
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

	//disable_interrupt();
	if (query_key_u(2)) accel = TRUE;
	if (query_key_u(3)) {
		release_key_u(2);
		release_key_u(3);
		accel = FALSE;
	}
	if (query_key_u(1)) {
		release_key_u(1);
		if (table_location > 2)
			if (accel)
				table_location = (table_location - 32 < 2) ? 2 : table_location - 32;
			else table_location = (table_location - 16 < 2) ? 2 : table_location - 16;
	} else if (query_key_u(0)) {
		release_key_u(0);
		if (table_location + table_length < SCR_WIDTH)
			if (accel)
				table_location =  (table_location + table_length + 32 > SCR_WIDTH) ?
				                  SCR_WIDTH - table_length - 1 : table_location + 32;
			else
				table_location =  (table_location + table_length + 16 > SCR_WIDTH) ?
				                  SCR_WIDTH - table_length - 1 : table_location + 16;
	}

	//enable_interrupt();

	return FALSE;
}
