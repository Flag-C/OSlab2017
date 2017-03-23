#include "x86/x86.h"
#include "common.h"

/* some ports */
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9

/* This is the number of entries in the palette. You may change this value to fit your palette. */
#define NR_PALETTE_ENTRY 256

/* This is an example. Change it to your palette if you like. */
static uint8_t palette[NR_PALETTE_ENTRY][3] = {
	{0X00, 0X00, 0X00}, {0X00, 0X00, 0X55}, {0X00, 0X00, 0XAA}, {0X00, 0X00, 0XFF},
	{0X00, 0X24, 0X00}, {0X00, 0X24, 0X55}, {0X00, 0X24, 0XAA}, {0X00, 0X24, 0XFF},
	{0X00, 0X48, 0X00}, {0X00, 0X48, 0X55}, {0X00, 0X48, 0XAA}, {0X00, 0X48, 0XFF},
	{0X00, 0X6D, 0X00}, {0X00, 0X6D, 0X55}, {0X00, 0X6D, 0XAA}, {0X00, 0X6D, 0XFF},
	{0X00, 0X91, 0X00}, {0X00, 0X91, 0X55}, {0X00, 0X91, 0XAA}, {0X00, 0X91, 0XFF},
	{0X00, 0XB6, 0X00}, {0X00, 0XB6, 0X55}, {0X00, 0XB6, 0XAA}, {0X00, 0XB6, 0XFF},
	{0X00, 0XDA, 0X00}, {0X00, 0XDA, 0X55}, {0X00, 0XDA, 0XAA}, {0X00, 0XDA, 0XFF},
	{0X00, 0XFF, 0X00}, {0X00, 0XFF, 0X55}, {0X00, 0XFF, 0XAA}, {0X00, 0XFF, 0XFF},
	{0X24, 0X00, 0X00}, {0X24, 0X00, 0X55}, {0X24, 0X00, 0XAA}, {0X24, 0X00, 0XFF},
	{0X24, 0X24, 0X00}, {0X24, 0X24, 0X55}, {0X24, 0X24, 0XAA}, {0X24, 0X24, 0XFF},
	{0X24, 0X48, 0X00}, {0X24, 0X48, 0X55}, {0X24, 0X48, 0XAA}, {0X24, 0X48, 0XFF},
	{0X24, 0X6D, 0X00}, {0X24, 0X6D, 0X55}, {0X24, 0X6D, 0XAA}, {0X24, 0X6D, 0XFF},
	{0X24, 0X91, 0X00}, {0X24, 0X91, 0X55}, {0X24, 0X91, 0XAA}, {0X24, 0X91, 0XFF},
	{0X24, 0XB6, 0X00}, {0X24, 0XB6, 0X55}, {0X24, 0XB6, 0XAA}, {0X24, 0XB6, 0XFF},
	{0X24, 0XDA, 0X00}, {0X24, 0XDA, 0X55}, {0X24, 0XDA, 0XAA}, {0X24, 0XDA, 0XFF},
	{0X24, 0XFF, 0X00}, {0X24, 0XFF, 0X55}, {0X24, 0XFF, 0XAA}, {0X24, 0XFF, 0XFF},
	{0X48, 0X00, 0X00}, {0X48, 0X00, 0X55}, {0X48, 0X00, 0XAA}, {0X48, 0X00, 0XFF},
	{0X48, 0X24, 0X00}, {0X48, 0X24, 0X55}, {0X48, 0X24, 0XAA}, {0X48, 0X24, 0XFF},
	{0X48, 0X48, 0X00}, {0X48, 0X48, 0X55}, {0X48, 0X48, 0XAA}, {0X48, 0X48, 0XFF},
	{0X48, 0X6D, 0X00}, {0X48, 0X6D, 0X55}, {0X48, 0X6D, 0XAA}, {0X48, 0X6D, 0XFF},
	{0X48, 0X91, 0X00}, {0X48, 0X91, 0X55}, {0X48, 0X91, 0XAA}, {0X48, 0X91, 0XFF},
	{0X48, 0XB6, 0X00}, {0X48, 0XB6, 0X55}, {0X48, 0XB6, 0XAA}, {0X48, 0XB6, 0XFF},
	{0X48, 0XDA, 0X00}, {0X48, 0XDA, 0X55}, {0X48, 0XDA, 0XAA}, {0X48, 0XDA, 0XFF},
	{0X48, 0XFF, 0X00}, {0X48, 0XFF, 0X55}, {0X48, 0XFF, 0XAA}, {0X48, 0XFF, 0XFF},
	{0X6D, 0X00, 0X00}, {0X6D, 0X00, 0X55}, {0X6D, 0X00, 0XAA}, {0X6D, 0X00, 0XFF},
	{0X6D, 0X24, 0X00}, {0X6D, 0X24, 0X55}, {0X6D, 0X24, 0XAA}, {0X6D, 0X24, 0XFF},
	{0X6D, 0X48, 0X00}, {0X6D, 0X48, 0X55}, {0X6D, 0X48, 0XAA}, {0X6D, 0X48, 0XFF},
	{0X6D, 0X6D, 0X00}, {0X6D, 0X6D, 0X55}, {0X6D, 0X6D, 0XAA}, {0X6D, 0X6D, 0XFF},
	{0X6D, 0X91, 0X00}, {0X6D, 0X91, 0X55}, {0X6D, 0X91, 0XAA}, {0X6D, 0X91, 0XFF},
	{0X6D, 0XB6, 0X00}, {0X6D, 0XB6, 0X55}, {0X6D, 0XB6, 0XAA}, {0X6D, 0XB6, 0XFF},
	{0X6D, 0XDA, 0X00}, {0X6D, 0XDA, 0X55}, {0X6D, 0XDA, 0XAA}, {0X6D, 0XDA, 0XFF},
	{0X6D, 0XFF, 0X00}, {0X6D, 0XFF, 0X55}, {0X6D, 0XFF, 0XAA}, {0X6D, 0XFF, 0XFF},
	{0X91, 0X00, 0X00}, {0X91, 0X00, 0X55}, {0X91, 0X00, 0XAA}, {0X91, 0X00, 0XFF},
	{0X91, 0X24, 0X00}, {0X91, 0X24, 0X55}, {0X91, 0X24, 0XAA}, {0X91, 0X24, 0XFF},
	{0X91, 0X48, 0X00}, {0X91, 0X48, 0X55}, {0X91, 0X48, 0XAA}, {0X91, 0X48, 0XFF},
	{0X91, 0X6D, 0X00}, {0X91, 0X6D, 0X55}, {0X91, 0X6D, 0XAA}, {0X91, 0X6D, 0XFF},
	{0X91, 0X91, 0X00}, {0X91, 0X91, 0X55}, {0X91, 0X91, 0XAA}, {0X91, 0X91, 0XFF},
	{0X91, 0XB6, 0X00}, {0X91, 0XB6, 0X55}, {0X91, 0XB6, 0XAA}, {0X91, 0XB6, 0XFF},
	{0X91, 0XDA, 0X00}, {0X91, 0XDA, 0X55}, {0X91, 0XDA, 0XAA}, {0X91, 0XDA, 0XFF},
	{0X91, 0XFF, 0X00}, {0X91, 0XFF, 0X55}, {0X91, 0XFF, 0XAA}, {0X91, 0XFF, 0XFF},
	{0XB6, 0X00, 0X00}, {0XB6, 0X00, 0X55}, {0XB6, 0X00, 0XAA}, {0XB6, 0X00, 0XFF},
	{0XB6, 0X24, 0X00}, {0XB6, 0X24, 0X55}, {0XB6, 0X24, 0XAA}, {0XB6, 0X24, 0XFF},
	{0XB6, 0X48, 0X00}, {0XB6, 0X48, 0X55}, {0XB6, 0X48, 0XAA}, {0XB6, 0X48, 0XFF},
	{0XB6, 0X6D, 0X00}, {0XB6, 0X6D, 0X55}, {0XB6, 0X6D, 0XAA}, {0XB6, 0X6D, 0XFF},
	{0XB6, 0X91, 0X00}, {0XB6, 0X91, 0X55}, {0XB6, 0X91, 0XAA}, {0XB6, 0X91, 0XFF},
	{0XB6, 0XB6, 0X00}, {0XB6, 0XB6, 0X55}, {0XB6, 0XB6, 0XAA}, {0XB6, 0XB6, 0XFF},
	{0XB6, 0XDA, 0X00}, {0XB6, 0XDA, 0X55}, {0XB6, 0XDA, 0XAA}, {0XB6, 0XDA, 0XFF},
	{0XB6, 0XFF, 0X00}, {0XB6, 0XFF, 0X55}, {0XB6, 0XFF, 0XAA}, {0XB6, 0XFF, 0XFF},
	{0XDA, 0X00, 0X00}, {0XDA, 0X00, 0X55}, {0XDA, 0X00, 0XAA}, {0XDA, 0X00, 0XFF},
	{0XDA, 0X24, 0X00}, {0XDA, 0X24, 0X55}, {0XDA, 0X24, 0XAA}, {0XDA, 0X24, 0XFF},
	{0XDA, 0X48, 0X00}, {0XDA, 0X48, 0X55}, {0XDA, 0X48, 0XAA}, {0XDA, 0X48, 0XFF},
	{0XDA, 0X6D, 0X00}, {0XDA, 0X6D, 0X55}, {0XDA, 0X6D, 0XAA}, {0XDA, 0X6D, 0XFF},
	{0XDA, 0X91, 0X00}, {0XDA, 0X91, 0X55}, {0XDA, 0X91, 0XAA}, {0XDA, 0X91, 0XFF},
	{0XDA, 0XB6, 0X00}, {0XDA, 0XB6, 0X55}, {0XDA, 0XB6, 0XAA}, {0XDA, 0XB6, 0XFF},
	{0XDA, 0XDA, 0X00}, {0XDA, 0XDA, 0X55}, {0XDA, 0XDA, 0XAA}, {0XDA, 0XDA, 0XFF},
	{0XDA, 0XFF, 0X00}, {0XDA, 0XFF, 0X55}, {0XDA, 0XFF, 0XAA}, {0XDA, 0XFF, 0XFF},
	{0XFF, 0X00, 0X00}, {0XFF, 0X00, 0X55}, {0XFF, 0X00, 0XAA}, {0XFF, 0X00, 0XFF},
	{0XFF, 0X24, 0X00}, {0XFF, 0X24, 0X55}, {0XFF, 0X24, 0XAA}, {0XFF, 0X24, 0XFF},
	{0XFF, 0X48, 0X00}, {0XFF, 0X48, 0X55}, {0XFF, 0X48, 0XAA}, {0XFF, 0X48, 0XFF},
	{0XFF, 0X6D, 0X00}, {0XFF, 0X6D, 0X55}, {0XFF, 0X6D, 0XAA}, {0XFF, 0X6D, 0XFF},
	{0XFF, 0X91, 0X00}, {0XFF, 0X91, 0X55}, {0XFF, 0X91, 0XAA}, {0XFF, 0X91, 0XFF},
	{0XFF, 0XB6, 0X00}, {0XFF, 0XB6, 0X55}, {0XFF, 0XB6, 0XAA}, {0XFF, 0XB6, 0XFF},
	{0XFF, 0XDA, 0X00}, {0XFF, 0XDA, 0X55}, {0XFF, 0XDA, 0XAA}, {0XFF, 0XDA, 0XFF},
	{0XFF, 0XFF, 0X00}, {0XFF, 0XFF, 0X55}, {0XFF, 0XFF, 0XAA}, {0XFF, 0XFF, 0XFF},
};

/* Load the palette into VGA.
 * If you want to use your own palette, replace the above palette with yours, then call this function in game_init().
 * But the blue screen may not be "blue" any longer. ^_^
 */
void write_palette()
{
	int i;
	out_byte(VGA_DAC_WRITE_INDEX, 0);
	for (i = 0; i < NR_PALETTE_ENTRY; i ++) {
		out_byte(VGA_DAC_DATA, palette[i][0] >> 2);	// red
		out_byte(VGA_DAC_DATA, palette[i][1] >> 2);	// green
		out_byte(VGA_DAC_DATA, palette[i][2] >> 2);	// blue
	}
}

/* Print the palette in use. */
void read_palette()
{
	int i;
	uint8_t r, g, b;
	out_byte(VGA_DAC_READ_INDEX, 0);
	for (i = 0; i < NR_PALETTE_ENTRY; i ++) {
		r = in_byte(VGA_DAC_DATA);
		g = in_byte(VGA_DAC_DATA);
		b = in_byte(VGA_DAC_DATA);
		printk("r = %x, g = %x, b = %x\n", r, g, b);
	}
}