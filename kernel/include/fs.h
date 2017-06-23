enum {
	SEEK_SET = 0,
	SEEK_CUR,
	SEEK_END
};

int sys_fopen(const char *path, const char *mode);
int sys_fread(void *dst, int size, int index);
int sys_fwrite(void *src, int size, int index);
int sys_fseek(int index, int offset, int whence);
int sys_fclose(int index);
