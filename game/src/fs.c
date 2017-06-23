int file_open(const char *path, const char *mode)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(18), "b"(path), "c"(mode));
	return ret;
}

int file_read(void *dst, int size, int index)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(19), "b"(dst), "c"(size), "d"(index));
	return ret;
}

int file_write(void *src, int size, int index)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(20), "b"(src), "c"(size), "d"(index));
	return ret;
}

int file_seek(int index, int offset, int whence)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(22), "b"(index), "c"(offset), "d"(whence));
	return ret;
}

int file_close(int index)
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(21), "b"(index));
	return ret;
}
