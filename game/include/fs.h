enum {
	SEEK_SET = 0,
	SEEK_CUR,
	SEEK_END
};

int file_open(const char *path, const char *mode);
int file_read(void *dst, int size, int index);
int file_write(void *src, int size, int index);
int file_close(int index);
int file_seek(int index, int offset, int whence);
