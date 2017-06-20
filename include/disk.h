typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

#define DISK_SZ (2*1024*1024) // measured by B
#define BLOCK_SZ 512 // measured by B
// DISK_SZ/BLOCK_SZ/8/BLOCK_SZ should be integer.

#define NR_BLOCK (DISK_SZ / BLOCK_SZ)
#define GET_BIT(bm,i,j) ((((bm).mask[i]) >> (j)) & 0x1)
#define SET_BIT(bm,i,j) ((bm).mask[i] |=(0x1 << (j)))

#define NR_ENTRY_PER_DIR (BLOCK_SZ / sizeof(Dirent))

typedef struct bitmap {
	uint8_t mask[NR_BLOCK / 8];
} Bitmap;

typedef struct dirent {
	char file_name[56];
	uint32_t file_size;
	uint32_t inode_offset;
} Dirent;

typedef struct dir {
	Dirent entries[NR_ENTRY_PER_DIR];
} Dir;

typedef struct inode {
	uint32_t data_block_offsets[BLOCK_SZ / sizeof(uint32_t)];
} Inode;

