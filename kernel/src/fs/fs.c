#include "string.h"
#include "fcb.h"
#include "fs.h"
#include "env.h"
#include "screen.h"

void readseg(unsigned char *, int, int);
void writeseg(unsigned char *, int, int);

Bitmap bitmap;
Dir rootdir;
uint32_t fs_offset_in_disk;

void init_fs(uint32_t offset_in_disk)
{
	readseg((void *)&bitmap, sizeof(bitmap), offset_in_disk);
	readseg((void *)&rootdir, sizeof(rootdir), offset_in_disk + sizeof(bitmap));
	fs_offset_in_disk = offset_in_disk;
}

int block_alloc()
{
	int i, j;
	for (i = 0; i < sizeof(bitmap); i ++)
		if (bitmap.mask[i] != 0xff)
			for (j = 0; j < 8; j ++)
				if (!GET_BIT(bitmap, i, j)) {
					SET_BIT(bitmap, i, j);
					return (i << 3) + j;
				}
	return -1;
}

static void bitmap_update()
{
	writeseg((void *)&bitmap, sizeof(bitmap), fs_offset_in_disk);
}

static void rootdir_update()
{
	writeseg((void *)&rootdir, sizeof(rootdir), fs_offset_in_disk + sizeof(bitmap));
}

int sys_fopen(const char *path, const char *mode)
{
	int i;
	FCB *fcbptr;
	if (curenv->env_nr_fcb == NR_FILE) {
		printk("Error: the current process has opened too many files.\n");
		return -1;
	}
	for (i = 0; i < NR_ENTRY_PER_DIR; i ++)
		if (strcmp(rootdir.entries[i].file_name, path) == 0)
			break;
	if (i == NR_ENTRY_PER_DIR) {
		printk("Error: file '%s' not found.\n", path);
		return -1;
	}
	if ((fcbptr = fcb_alloc(rootdir.entries[i].inode_offset, mode[0])) == NULL)
		return -1;
	fcbptr->fcb_file_sz = rootdir.entries[i].file_size;
	for (i = 0; i < NR_FILE; i ++)
		if (curenv->env_fcb_index[i] == -1) {
			curenv->env_fcb_index[i] = fcbptr - fcbs;
			break;
		}
	printk("File '%s' was opened successfully.\n", path);
	return fcbptr - fcbs;
}

int sys_fread(void *dst, int size, int index)
{
	FCB *curfcb = &fcbs[index];
	int seek = curfcb->fcb_seek, end, read_size;
	if (curfcb->fcb_mode != 'r') {
		printk("Error: file read permission declined.\n");
		return -1;
	}
	size = (size < curfcb->fcb_file_sz - seek) ? size : (curfcb->fcb_file_sz - seek);
	end = seek + size;
	for (; seek < end; ) {
		read_size = (end - seek < BLOCK_SZ - (seek % BLOCK_SZ)) ? (end - seek) : (BLOCK_SZ - (seek % BLOCK_SZ));
		fcb_read(dst, curfcb, seek / BLOCK_SZ, seek % BLOCK_SZ, read_size);
		seek += read_size;
		dst += read_size;
	}
	curfcb->fcb_seek = seek;
	return size;
}

int sys_fwrite(void *src, int size, int index)
{
	FCB *curfcb = &fcbs[index];
	int seek = curfcb->fcb_seek, end, write_size;
	if (curfcb->fcb_mode != 'w') {
		printk("Error: file write permission declined.\n");
		return -1;
	}
	if (curfcb->fcb_file_sz + size > MAX_FILE_SZ) {
		printk("Error: file size exceeded maximum value(%d bytes).\n", MAX_FILE_SZ);
		return -1;
	}
	end = seek + size;
	for (; seek < end; ) {
		write_size = (end - seek < BLOCK_SZ - (seek % BLOCK_SZ)) ? (end - seek) : (BLOCK_SZ - (seek % BLOCK_SZ));
		fcb_write(src, curfcb, seek / BLOCK_SZ, seek % BLOCK_SZ, write_size);
		seek += write_size;
		src += write_size;
	}
	curfcb->fcb_seek = seek;
	bitmap_update();
	if (end > curfcb->fcb_file_sz) {
		curfcb->fcb_file_sz = end;
		for (seek = 0; seek < NR_FILE; seek ++) {
			if (rootdir.entries[seek].inode_offset == curfcb->fcb_inode_offset) {
				rootdir.entries[seek].file_size = end;
				rootdir_update();
				break;
			}
		}
	}
	return size;
}
int sys_fseek(int index, int offset, int whence)
{
	FCB *curfcb = &fcbs[index];
	int end;
	if (whence == SEEK_SET)
		end = offset;
	else if (whence == SEEK_CUR)
		end = curfcb->fcb_seek + offset;
	else if (whence == SEEK_END)
		end = curfcb->fcb_file_sz + offset;
	else {
		printk("Error: whence should be SEEK_SET or SEEK_CUR or SEEK_END!\n");
		return -1;
	}
	if (end < 0) {
		printk("Error: the location sought is below 0!\n");
		return -1;
	}
	if (end > curfcb->fcb_file_sz) {
		printk("Error: the location sought exceeds file size!\n");
		return -1;
	}
	curfcb->fcb_seek = end;
	return 0;
}
int sys_fclose(int index)
{
	int i;
	FCB *curfcb = &fcbs[index];
	for (i = 0; i < NR_FILE; i ++) {
		if (curenv->env_fcb_index[i] == index) {
			curenv->env_fcb_index[i] = -1;
			break;
		}
	}
	fcb_free(curfcb);
	return 0;
}

void sys_show_rootdir(bool ls_l, bool ls_a)
{
	int i, j, sz;
	char sz_ch[23] = "                      ";
	if (!ls_l) {
		if (ls_a)
			screen_print_string(".  ..  ", 7);
		for (i = 0; i < NR_ENTRY_PER_DIR; i ++)
			if (rootdir.entries[i].file_size > 0) {
				screen_print_string(rootdir.entries[i].file_name, 7);
				screen_print_string("  ", 7);
			}
		screen_print_string("\n", 7);
	} else {
		if (ls_a)
			screen_print_string(".               4096  00:00:00\n..              4096  00:00:00\n", 7);
		for (i = 0; i < NR_ENTRY_PER_DIR; i ++) {
			if (rootdir.entries[i].file_size > 0) {
				screen_print_string(rootdir.entries[i].file_name, 7);
				j = 19 - strlen(rootdir.entries[i].file_name);
				sz = rootdir.entries[i].file_size;
				strcpy(sz_ch, "                      ");
				sz_ch[j + 3] = 0;
				while (sz > 0) {
					sz_ch[j] = sz % 10 + '0';
					sz /= 10;
					j --;
				}
				screen_print_string(sz_ch, 7);
				unsigned t = rootdir.entries[i].file_time;
				strcpy(sz_ch, "  :  :  \n");
				sz_ch[0] = '0' + t / 36000;
				sz_ch[1] = '0' + (t / 3600) % 10;
				sz_ch[3] = '0' + ((t / 60) % 60) / 10;
				sz_ch[4] = '0' + ((t / 60) % 60) % 10;
				sz_ch[6] = '0' + (t % 60) / 10;
				sz_ch[7] = '0' + t % 10;
				screen_print_string(sz_ch, 7);
			}
		}
	}
}

void schedule_process();
void sys_frun(char name[])
{
	int i;
	char dst[BLOCK_SZ];
	for (i = 0; i < NR_ENTRY_PER_DIR; i ++)
		if (strcmp(rootdir.entries[i].file_name, name) == 0) {
			readseg((void *)dst, BLOCK_SZ, fs_offset_in_disk + rootdir.entries[i].inode_offset * BLOCK_SZ);
			env_create(fs_offset_in_disk + * ((int *)dst)*BLOCK_SZ, 0);
			schedule_process();
			break;
		}
}

unsigned get_tick();
void sys_ftouch(char name[])
{
	int i;
	for (i = 0; i < NR_ENTRY_PER_DIR; i ++)
		if (strcmp(rootdir.entries[i].file_name, name) == 0) {
			rootdir.entries[i].file_time = get_tick();
			break;
		}
}