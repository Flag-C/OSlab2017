#include "types.h"
#include "disk.h"

#define LOG2NFCB	5
#define NFCB		(1 << LOG2NFCB)

typedef struct FCB {
	int fcb_inode_offset;
	Inode fcb_inode;
	char fcb_mode;
	int fcb_seek;
	uint32_t fcb_file_sz;
	int fcb_buf_block;
	uint8_t fcb_buffer[BLOCK_SZ];
	struct	FCB *fcb_link;
} FCB;

extern FCB *fcbs;
extern uint32_t fs_offset_in_disk;

void fcb_init(void);
FCB *fcb_alloc(uint32_t inode_offset, char mode);
void fcb_update(FCB *fcbptr);
void fcb_read(void *dst, FCB *curfcb, int block_index, int start, int size);
void fcb_write(void *src, FCB *curfcb, int block_index, int start, int size);
void fcb_free(FCB *curfcb);
