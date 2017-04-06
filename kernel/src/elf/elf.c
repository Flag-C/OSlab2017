#include "elf.h"
#define SECTSIZE 512
#define ELF_OFFSET_IN_DISK 102400
//refer to ics pa

void readseg(unsigned char *, int, int);

unsigned int
loader(void)
{
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char* pa, *i;

	elf = (struct ELFHeader*)0x0;

	/* read elf header */
	readseg((void *)elf, 4096, ELF_OFFSET_IN_DISK);

	/* loader */
	ph = (struct ProgramHeader *)((void *)elf + elf->phoff);
	eph = ph + elf->phnum;
	for (; ph < eph; ph ++) {
		pa = (void *)ph->paddr;
		readseg(pa, ph->filesz, ELF_OFFSET_IN_DISK + ph->off);
		for (i = pa + ph->filesz; i < pa + ph->memsz; *i ++ = 0);
	}

	/* entry */
	volatile unsigned int entry = elf->entry;
	return entry;
}

void
waitdisk(void)
{
	while ((in_byte(0x1F7) & 0xC0) != 0x40);
}

void
readsect(void *dst, int offset)
{
	int i;
	waitdisk();
	out_byte(0x1F2, 1);
	out_byte(0x1F3, offset);
	out_byte(0x1F4, offset >> 8);
	out_byte(0x1F5, offset >> 16);
	out_byte(0x1F6, (offset >> 24) | 0xE0);
	out_byte(0x1F7, 0x20);

	waitdisk();
	for (i = 0; i < SECTSIZE / 4; i ++)
		((int *)dst)[i] = in_long(0x1F0);
}

void
readseg(unsigned char *pa, int count, int offset)
{
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + 1;
	for (; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}