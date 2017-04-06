#include "common.h"
#include "irq.h"
#include "x86/x86.h"
#include "device/video.h"
#include "device/timer.h"
#include "device/font.h"
#include "device/palette.h"

void init_serial();

void timer_event(void);
void keyboard_event(int scan_code);

int main(void)
{
    init_serial();
    init_timer();
    init_idt();
    init_intr();
    set_timer_intr_handler(timer_event);
    set_keyboard_intr_handler(keyboard_event);
    write_palette();
    printk("game start!\n");
    enable_interrupt();
    unsigned int eip = loader();
    ((void(*)(void))eip)();
    printk("kernel shouldn't reach here \n");
}