#include "disk.h"
#include "string.h"

void readseg(unsigned char *, int, int);

Bitmap bitmap;
Dir rootdir;

void init_fs(uint32_t offset_in_disk)
{
	readseg((void *)&bitmap, sizeof(bitmap), offset_in_disk);
	readseg((void *)&rootdir, sizeof(rootdir), offset_in_disk + sizeof(bitmap));
}
