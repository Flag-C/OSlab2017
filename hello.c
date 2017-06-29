void exit(int stat)
{
	asm volatile("int $0x80" : : "a"(11), "b"(stat));
}

void shell_print_string(char str[], int color)
{
	asm volatile("int $0x80" : : "a"(25), "b"(str), "c"(color));
}

int main()
{
	shell_print_string("Hello world!\n", 7);
	exit(0);
}
