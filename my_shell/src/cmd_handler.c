#include "string.h"
#include "types.h"
#include "../include/lib.h"
#include "../include/cmd.h"

static bool ls_a = false, ls_l = false;
void cmd_ls(const char cmd[])
{
	int i = 0;
	for (; cmd[i] == ' '; i ++);
	if (cmd[i] == '\0' || cmd[i] == '\n')
		return;
	if (cmd[i] == '-') {
		if (cmd[i + 1] == 'a') {
			ls_a = true;
			cmd_ls(cmd + i + 2);
		} else if (cmd[1] == 'l') {
			ls_l = true;
			cmd_ls(cmd + i + 2);
		}
	}
}

void cmd_handler(const char cmd[])
{
	int i = 0;
	for (; cmd[i] == ' '; i ++);
	if (cmd[i] == 'l' && cmd[i + 1] == 's') {
		ls_a = false;
		ls_l = false;
		cmd_ls(cmd + i + 2);

		char res[512];
		show_rootdir(ls_l, ls_a, res);

		for (i = 0; res[i]; i ++) {
			shell_print_string(res + i, 9);
			shell_print_string(" ", 9);
			i += strlen(res + i);
		}
		shell_enter();
		printf("\n");
	}
}

