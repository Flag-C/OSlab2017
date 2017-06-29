#include "string.h"
#include "x86/x86.h"
#include "../include/lib.h"
#include "../include/keyboard.h"
#include "../include/fs.h"

void cmd_handler(const char cmd[]);

const char key_sequence[];

int main()
{
	printf("Hi! I'm shell!\n");

	bool key_pressing = false;
	int cur_key;
	char input_str[50] = "";
	int  input_len = 0;
	init_screen();

	shell_print_string("FlagC@MyShell:~$ ", 3);
	while (1) {
		if (!key_pressing) {
			cur_key = see_if_any_key_pressed();
			if (cur_key != -1) {
				key_pressing = true;
				input_str[input_len] = key_sequence[cur_key];
				shell_print_string(input_str + input_len, 7);
				input_len ++;
				if (cur_key == KEY_ENTER) {
					cmd_handler(input_str);
					memset(input_str, 0, sizeof(input_str));
					input_len = 0;
					shell_print_string("FlagC@MyShell:~$ ", 3);
				}
			}
		} else {
			if (!query_key_u(cur_key))
				key_pressing = false;
		}
	}
}
