#include "types.h"
#include "disk.h"

#define LOG2NFCB	5
#define NFCB		(1 << LOG2NFCB)

typedef struct FCB {
	Inode fcb_inode;
	char fcb_type;
	int fcb_bufblock;
	uint8_t fcb_buffer[BLOCK_SZ];
	struct	FCB *fcb_link;
} FCB;

extern FCB *fcbs;

void fcb_init(void);
