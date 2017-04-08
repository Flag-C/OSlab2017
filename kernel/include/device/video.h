#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "common.h"

#define SCR_WIDTH  320
#define SCR_HEIGHT 200
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
#define VMEM_ADDR  ((uint8_t*)0xc00a0000)

extern uint8_t *vmem;

void prepare_buffer(void);
void display_buffer(void);
#endif
