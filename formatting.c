#include <stdio.h>
#include <string.h>
#include "include/disk.h"

Bitmap bitmap;
Dir rootdir;

static void init_bitmap()
{
	int i, nr_bitmap_block = sizeof(bitmap) / BLOCK_SZ;
	memset(&bitmap, 0, sizeof(bitmap));
	memset(&bitmap, 0xff, nr_bitmap_block / 8);
	for (i = 0; i < nr_bitmap_block % 8; i ++)
		SET_BIT(bitmap, nr_bitmap_block / 8, i);
}

static void init_rootdir()
{
	int i, nr_bitmap_block = sizeof(bitmap) / BLOCK_SZ;
	SET_BIT(bitmap, nr_bitmap_block / 8, nr_bitmap_block % 8);
	memset(&rootdir, 0, sizeof(rootdir));
	for (i = 0; i < NR_ENTRY_PER_DIR; i ++)
		rootdir.entries[i].inode_offset = -1;
}

static int block_alloc()
{
	int i, j;
	for (i = 0; i < sizeof(bitmap); i++)
		if (bitmap.mask[i] != 0xff)
			for (j = 0; j < 8; j ++)
				if (!GET_BIT(bitmap, i, j)) {
					SET_BIT(bitmap, i, j);
					return (i << 3) + j;
				}
	return -1;
}

int main(int argc, char *argv[])
{
	int i, cnt, block_index, inode_block_index;
	uint8_t buf[BLOCK_SZ];
	FILE *diskfp, *fp;
	Inode inode;
	if (argc < 3) {
		printf("Error: not enough arguments.\n");
		return 1;
	}
	init_bitmap();
	init_rootdir();

	if ((diskfp = fopen(argv[1], "wb")) == NULL) {
		printf("Error: disk creation failed.\n");
		return 2;
	}
	for (i = 2; i < argc; i++) {
		if ((fp = fopen(argv[i], "rb")) == NULL) {
			printf("Error: file '%s' open failed.\n", argv[i]);
			return 2;
		}
		if ((rootdir.entries[i - 2].inode_offset = block_alloc()) == -1) {
			printf("Error: disk image full, formatting of file '%s' failed.\n", argv[i]);
			return 3;
		}
		cnt = 0;
		memset(&inode, 0xff, sizeof(inode));
		fseek(fp, 0, SEEK_END);
		rootdir.entries[i - 2].file_size = ftell(fp);
		strcpy(rootdir.entries[i - 2].file_name, argv[i]);
		fseek(fp, 0, SEEK_SET);
		while (fread(buf, 1, BLOCK_SZ, fp)) {
			if ((inode.data_block_offsets[cnt] = block_alloc()) == -1) {
				printf("Error: disk image full, formatting of file '%s' failed.\n", argv[i]);
				return 3;
			}
			fseek(diskfp, inode.data_block_offsets[cnt] * BLOCK_SZ, SEEK_SET);
			fwrite(buf, 1, BLOCK_SZ, diskfp);
			cnt++;
		}
		fclose(fp);
		fseek(diskfp, rootdir.entries[i - 2].inode_offset * BLOCK_SZ, SEEK_SET);
		fwrite(&inode, 1, BLOCK_SZ, diskfp);
		printf("File '%s' stored at block offset %d\n", rootdir.entries[i - 2].file_name, inode.data_block_offsets[0]);
	}
	fseek(diskfp, sizeof(bitmap), SEEK_SET);
	fwrite(&rootdir, 1, BLOCK_SZ, diskfp);
	fseek(diskfp, 0, SEEK_SET);
	fwrite(&bitmap, 1, BLOCK_SZ, diskfp);
	fseek(diskfp, DISK_SZ - 1, SEEK_SET);
	fprintf(diskfp, "\0");
	fclose(diskfp);
	return 0;
}
