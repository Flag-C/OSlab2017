#include "string.h"
#include "fcb.h"
#include "env.h"

FCB foo2[NFCB];
FCB *fcbs = foo2;
static FCB *fcb_free_list;
void readseg(unsigned char *, int, int);
void writeseg(unsigned char *, int, int);
int block_alloc();

void fcb_init()
{
	int i;
	memset((void *)fcbs, 0, NFCB * sizeof(FCB));
	for (i = NFCB - 1; i >= 0; i --) {
		fcbs[i].fcb_inode_offset = -1;
		fcbs[i].fcb_link = fcb_free_list;
		fcb_free_list = fcbs + i;
	}
}

// fcb_file_sz not initialized.
FCB *fcb_alloc(uint32_t inode_offset, char mode)
{
	FCB *fcbptr;
	if (fcb_free_list == NULL) {
		printk("Error: too many files are opened.\n");
		return NULL;
	}
	fcbptr = fcb_free_list;
	fcb_free_list = fcbptr->fcb_link;
	fcbptr->fcb_inode_offset = inode_offset;
	readseg((void *) & (fcbptr->fcb_inode), sizeof(Inode), fs_offset_in_disk + inode_offset * BLOCK_SZ);
	fcbptr->fcb_mode = mode;
	fcbptr->fcb_seek = 0;
	fcbptr->fcb_buf_block = 0;
	readseg((void *) & (fcbptr->fcb_buffer), BLOCK_SZ, fs_offset_in_disk + fcbptr->fcb_inode.data_block_offsets[0] * BLOCK_SZ);
	return fcbptr;
}

void fcb_update(FCB *fcbptr)
{
	writeseg((void *) & (fcbptr->fcb_buffer), BLOCK_SZ, fs_offset_in_disk + fcbptr->fcb_inode.data_block_offsets[fcbptr->fcb_buf_block] * BLOCK_SZ);
}

static void fcb_switch_buf(FCB *fcbptr, int block_index)
{
	fcb_update(fcbptr);
	fcbptr->fcb_buf_block = block_index;
	if (fcbptr->fcb_inode.data_block_offsets[block_index] == -1) {
		fcbptr->fcb_inode.data_block_offsets[block_index] = block_alloc();
		writeseg((void *)&fcbptr->fcb_inode.data_block_offsets[block_index], sizeof(uint32_t), fs_offset_in_disk + fcbptr->fcb_inode_offset * BLOCK_SZ + block_index * sizeof(uint32_t));
	}
	readseg((void *) & (fcbptr->fcb_buffer), BLOCK_SZ, fs_offset_in_disk + fcbptr->fcb_inode.data_block_offsets[block_index] * BLOCK_SZ);
}

// Data to be read should be in the same block.
void fcb_read(void *dst, FCB *curfcb, int block_index, int start, int size)
{
	int i;
	if (curfcb->fcb_buf_block != block_index)
		fcb_switch_buf(curfcb, block_index);
	for (i = 0; i < size; i ++)
		((uint8_t *)dst)[i] = curfcb->fcb_buffer[i + start];
}

void fcb_write(void *src, FCB *curfcb, int block_index, int start, int size)
{
	int i;
	if (curfcb->fcb_buf_block != block_index)
		fcb_switch_buf(curfcb, block_index);
	for (i = 0; i < size; i ++)
		curfcb->fcb_buffer[i + start] = ((uint8_t *)src)[i];
}

void fcb_free(FCB *curfcb)
{
	memset(curfcb, 0, sizeof(curfcb));
	curfcb->fcb_link = fcb_free_list;
	fcb_free_list = curfcb;
}