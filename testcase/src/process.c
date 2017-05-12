int fork()
{
	int ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(9));
	return ret;
}

void sleep(int t)
{
	asm volatile("int $0x80" : : "a"(10), "b"(t));
	return;
}

void exit(int stat)
{
	asm volatile("int $0x80" : : "a"(11), "b"(stat));
	return;
}
