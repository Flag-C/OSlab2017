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
		if (content[i] == '>')
			break;
		new[newi++] = content[i];
		if (content[i] == ' ')
			for (; content[i] == ' '; i++);
		else
			i++;
	}
	new[newi] = '\0';
	if (content[i] == '>') {
		if (content[i + 1] == '>') {
			char filename[52] = {0};
			int fi = 0;
			for (i = i + 2; content[i] == ' '; i++);
			for (; content[i] != '\n'; i++)
				filename[fi++] = content[i];
			int fp;
			fp = file_open(filename, "w");
			file_seek(fp, 0, SEEK_END);
			file_write(new, strlen(new), fp);
			file_close(fp);
		} else {
			char filename[52] = {0};
			int fi = 0;
			for (i = i + 1; content[i] == ' '; i++);
			for (; content[i] != '\n'; i++)
				filename[fi++] = content[i];
			int fp;
			fp = file_open(filename, "w");
			file_seek(fp, 0, SEEK_SET);
			file_write(new, strlen(new) + 1, fp);
			file_close(fp);

		}
	} else
		shell_print_string(new, 7);
}

bool cat_r = false;
void cmd_cat(char const content[], int fp_out)
{
	char new[50] = {0};
	int i, sz, fp;
	for (i = 0; content[i] != ' ' && content[i] != '\n' && content[i] != '>'; i++)
		new[i] = content[i];
	new[i] = 0;
	fp = file_open(new, "r");
	char dst[65] = {0};
	while ((sz = file_read(dst, 64, fp)) == 64)
		if (cat_r)
			file_write(dst, 64, fp_out);
		else
			shell_print_string(dst, 7);
	dst[sz] = 0;
	if (cat_r)
		file_write(dst, 64, fp_out);
	else
		shell_print_string(dst, 7);
	file_close(fp);
	for (; content[i] == ' '; i ++);
	if (content[i] == '\n' || content[i] == 0 || content[i] == '>')
		return;
	cmd_cat((char *)content + i, fp_out);
}

void cmd_run(const char content[])
{
	char new[50] = {0};
	strcpy(new, content);
	new[strlen(new) - 1] = 0; // remove '\n'
	int fp = file_open(new, "r");
	if (fp == -1) {
		shell_print_string("The file does not exist!\n", 7);
		return;
	}
	char magic[5] = {0};
	file_read(magic, 4, fp);
	if (magic[0] != 0x7f || magic[1] != 'E' || magic[2] != 'L' || magic[3] != 'F') {
		shell_print_string("The file is not executable!\n", 7);
	} else
		asm volatile("int $0x80" : : "a"(26), "b"(new));
	file_close(fp);
	return;
}

void cmd_touch(const char content[])
{
	char new[50] = {0};
	strcpy(new, content);
	new[strlen(new) - 1] = 0; // remove '\n'
	int fp = file_open(new, "r");
	if (fp == -1) {
		shell_print_string("The file does not exist!\n", 7);
		return;
	}
	asm volatile("int $0x80" : : "a"(27), "b"(new));
	file_close(fp);
}

void cmd_handler(char cmd[])
{
	int i = 0, j;
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
	} else if (cmd[i] == 'c' && cmd[i + 1] == 'a' && cmd[i + 2] == 't') {
		cat_r = false;
		int fp;
		for (j = 0; j < strlen(cmd); j++)
			if (cmd[j] == '>') {
				cat_r = true;
				if (cmd[j + 1] == '>') {
					char filename[52] = {0};
					int fi = 0;
					for (j = j + 2; cmd[j] == ' '; j++);
					for (; cmd[j] != '\n'; j++)
						filename[fi++] = cmd[j];
					fp = file_open(filename, "w");
					file_seek(fp, 0, SEEK_END);
				} else {
					char filename[52] = {0};
					int fi = 0;
					for (j = j + 1; cmd[j] == ' '; j++);
					for (; cmd[j] != '\n'; j++)
						filename[fi++] = cmd[j];
					fp = file_open(filename, "w");
					file_seek(fp, 0, SEEK_SET);
				}
				break;
			}
		for (i = i + 3; cmd[i] == ' '; i ++);
		cmd_cat(cmd + i, fp);
		if (cat_r)
			file_close(fp);
		shell_print_string("\n", 7);
	} else if (cmd[i] == 'r' && cmd[i + 1] == 'u' && cmd[i + 2] == 'n') {
		for (i = i + 3; cmd[i] == ' '; i ++);
		cmd_run(cmd + i);
	} else if (cmd[i] == 't' && cmd[i + 1] == 'o' && cmd[i + 2] == 'u' && cmd[i + 3] == 'c' && cmd[i + 4] == 'h') {
		for (i = i + 5; cmd[i] == ' '; i ++);
		cmd_touch(cmd + i);
	}
}

