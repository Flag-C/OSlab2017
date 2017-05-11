#include "common.h"
#include "irq.h"
#include "x86/x86.h"
#include "device/video.h"
#include "device/timer.h"
#include "device/font.h"
#include "device/palette.h"
#include "env.h"

void init_serial();
void page_init();
void init_segment();
void env_init();
void timer_event(void);
void keyboard_event(int scan_code);

int main(void)
{
    page_init();
    init_idt();
    init_intr();
    env_init();
    init_segment();
    init_serial();
    init_timer();
    set_timer_intr_handler(timer_event);
    set_keyboard_intr_handler(keyboard_event);
    write_palette();
    printk("game start!\n");
    enable_interrupt();

    set_tss_esp0(0xc0280000 - 4);
    int eid = env_create(4096, 0);
    struct Env *e;
    envid2env(eid, &e, false);
    env_run(e);
    printk("kernel shouldn't reach here \n");

    unsigned int eip = loader();
    run_game(eip);
}

void run_game(uintptr_t entry)
{
    struct TrapFrame *tf = (void *)USTACKTOP - sizeof(struct TrapFrame) - 4;
    tf->edi = 0;
    tf->esi = 0;
    tf->ebp = 0;
    tf->xxx = 0;
    tf->ebx = 0;
    tf->edx = 0;
    tf->ecx = 0;
    tf->eax = 0;
    tf->eflags = 0x202;
    tf->irq = 0x80;
    tf->eip = entry;
    tf->cs = (SELECTOR_USER(SEG_USER_CODE));
    set_tss_esp0(0xc0100000);
    tf->esp = USTACKTOP - 4;
    tf->ss = (SELECTOR_USER(SEG_USER_DATA));
    tf->ds = tf->es = (SELECTOR_USER(SEG_USER_DATA));
    asm volatile("movl %0, %%esp" : : "r"(tf));
    asm volatile("popa");
    asm volatile("popl %es");
    asm volatile("popl %ds");
    asm volatile("addl $8, %%esp" : :);
    printk("%s, %d: hahaha\n", __FUNCTION__, __LINE__);
    asm volatile("iret");
}