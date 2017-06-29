#include "string.h"
#include "types.h"
#include "../include/lib.h"
#include "../include/cmd.h"
#include "../include/fs.h"


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
		} else if (cmd[i + 1] == 'l') {
			ls_l = true;
			cmd_ls(cmd + i + 2);
		}
	}
}

void cmd_echo(const char content[])
{
	int i, newi = 0;
	char new[512];
	for (i = 0; i < strlen(content);) {
		new[newi++] = content[i];
		if (content[i] == ' ')
			for (; content[i] == ' '; i++);
		else
			i++;
	}
	new[newi] = '\0';
	shell_print_string(new, 7);
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
		show_rootdir(ls_l, ls_a);
	} else if (cmd[i] == 'e' && cmd[i + 1] == 'c' && cmd[i + 2] == 'h' && cmd[i + 3] == 'o') {
		for (i = i + 4; cmd[i] == ' '; i ++);
		cmd_echo(cmd + i);
	}
}

