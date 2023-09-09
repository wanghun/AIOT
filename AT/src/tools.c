#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "atbm_tool.h"

char cmd_line[CMD_LINE_LEN];

void MAC_printf(char mac[6])
{
	printf("%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int set_ps_cmd(int fp, int argc, char *argv[])
{

		printf("set_ps variables:\n"
	       "  ps_level (power save level, refer: 0/1/2\n"
	       "  ps_type (power save mode, refer: NO_SLEEP/MODEM_SLEEP/LIGHT_SLEEP/DEEP_SLEEP)\n");
        return 0;
}


int get_sdk_ver_cmd(int fp, int argc, char *argv[])
{
	int ret = 0;
	struct version_info version;

	printf("[SDK Version]\n %s\n", version.sdk_ver);
	return 0;

}

int user_help(int fp, int argc, char *argv[]);

struct COMMAND_STR command[] = {
    {"set_ps",set_ps_cmd,"<variable> <value> = set power save variables" },
    {"sdk_ver",get_sdk_ver_cmd,"get_sdk_ver_cmd" },
    {"help", user_help,"= show usage help"},
};

int user_help(int fp, int argc, char *argv[])
{
	int i = 0;
	int cnt = sizeof(command)/sizeof(command[0]);

	for (i=0; i<cnt; i++)
		printf("%s\n%s\n\n", command[i].cmd, command[i].descript);

	return 0;
}

static int tokenize_cmd(char *cmd, char *argv[], int is_at_cmd)
{
	char *pos;
	int argc = 0;

	pos = cmd;
	for (;;) {
		while (*pos == ' ')
			pos++;
		if (*pos == '\0')
			break;
		if (*pos == '"') {
			argv[argc] = pos + 1;
			char *pos2 = strrchr(pos, '"');
			if (pos2) {
				*pos2 = '\0';
				pos = pos2 + 1;
			}
		}
		else {
			argv[argc] = pos;
		}
		argc++;
		if (argc == MAX_ARGS)
			break;
		if (is_at_cmd)
		{
			if (argc > 1)
				break;
		}
		while (*pos != '\0' && *pos != ' ')
			pos++;
		if (*pos == ' ')
			*pos++ = '\0';
	}

	return argc;
}

int cmd_parse(int fp, char *arg)
{
	int i = 0;
	int cnt = sizeof(command)/sizeof(command[0]);
	char *argv[MAX_ARGS];		
	int argc;
	int ret = 0;

	if (!strncmp(arg, "fw_cmd ", strlen("fw_cmd ")))
	{
		argc = tokenize_cmd(arg, argv, 1);
		if (argc != 2)
		{
			printf("FW_CMD err.\n");
			return -1;
		}
	}
	else
	{
		argc = tokenize_cmd(arg, argv, 0);
	}
	if (argc)
	{
		for (i=0; i<cnt; i++)
		{
			if (!strcmp(command[i].cmd, argv[0]))
			{
				ret = command[i].cmd_send(fp, argc-1, &argv[1]);
				if (ret)
				{
					printf("FAIL\n");
				}
				else
				{
					printf("OK\n");
				}
				return 0;
			}
		}
	}
	
	printf("Line:%d not match command\n", __LINE__);
	return -1;
}

int at_main(int argc, char *argv[])
{


    //pthread_create(&tid, NULL, get_power_status_func, (void *)fp_power);
    //pthread_create(&sock_tid, NULL, get_command_func, NULL);

    int fp ;
    strcpy(cmd_line,"set_ps");
    cmd_parse(fp, cmd_line);

	return 0;
}


