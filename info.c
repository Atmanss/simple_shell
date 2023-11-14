#include "shell.h"

/**
 * clear_info - initializes info_t struct
 * @info: struct address
 */
void ClearInfo(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void SetInfo(info_t *info, char **av)
{
	int i = 0;

	info->File_Name = av[0];
	if (info->arg)
	{
		info->argv = StrToWordsFunction(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = StrDupFunction(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		ReplaceAlias(info);
		ReplaceVariables(info);
	}
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void FreeInfo(info_t *info, int all)
{
	FreeFunction(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			FreeList(&(info->env));
		if (info->history)
			FreeList(&(info->history));
		if (info->alias)
			FreeList(&(info->alias));
		FreeFunction(info->environ);
			info->environ = NULL;
		BFreeFunction((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BufferFlush);
	}
}
