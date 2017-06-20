int sys_fopen(const char *path, const char *mode);
int sys_fread(void *dst, int size, int index);
int sys_fwrite(void *src, int size, int index);
int sys_fclose(int index);