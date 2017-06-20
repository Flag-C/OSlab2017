/* ELF32 header */
struct ELFHeader {
	unsigned int   magic;
	unsigned char  elf[12];
	unsigned short type;
	unsigned short machine;
	unsigned int   version;
	unsigned int   entry;
	unsigned int   phoff;
	unsigned int   shoff;
	unsigned int   flags;
	unsigned short ehsize;
	unsigned short phentsize;
	unsigned short phnum;
	unsigned short shentsize;
	unsigned short shnum;
	unsigned short shstrndx;
};

/* ELF32 Program header */
struct ProgramHeader {
	unsigned int type;
	unsigned int off;
	unsigned int vaddr;
	unsigned int paddr;
	unsigned int filesz;
	unsigned int memsz;
	unsigned int flags;
	unsigned int align;
};

/* I/O function */

static inline int
in_long(short port)
{
	int data;
	asm volatile("in %1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline void
out_long(short port, int data)
{
	asm volatile("outl %0,%1" : : "a" (data), "d" (port));
}

void load_udir(pde_t *pgdir);