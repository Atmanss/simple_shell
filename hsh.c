#include "shell.h"

/**
 * HshFunction - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int HshFunction(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		ClearInfo(info);
		if (InteractiveFunction(info))
			PutsFunction("$ ");
		_eputchar(BufferFlush);
		r = GetInput(info);
		if (r != -1)
		{
			SetInfo(info, av);
			builtin_ret = FindBuiltinFunction(info);
			if (builtin_ret == -1)
				FindCmdFunction(info);
		}
		else if (InteractiveFunction(info))
			_putchar('\n');
		FreeInfo(info, 0);
	}
	WriteHistory(info);
	FreeInfo(info, 1);
	if (!InteractiveFunction(info) && info->Status)
		exit(info->Status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->Status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int FindBuiltinFunction(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", MyExit},
		{"env", MyEnv},
		{"help", MyHelp},
		{"history", MyHistory},
		{"setenv", MySetEnv},
		{"unsetenv", MyUnSetEnv},
		{"cd", MyCd},
		{"alias", MyAlias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (StrCmpFunction(info->argv[0], builtintbl[i].type) == 0)
		{
			info->LineCount++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void FindCmdFunction(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->Line_CountFlag == 1)
	{
		info->LineCount++;
		info->Line_CountFlag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!IsDelimiter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = FindPathFunction(info, GetEnv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		ForkCmdFunction(info);
	}
	else
	{
		if ((InteractiveFunction(info) || GetEnv(info, "PATH=")
			|| info->argv[0][0] == '/') && IsCmdFunction(info, info->argv[0]))
			ForkCmdFunction(info);
		else if (*(info->arg) != '\n')
		{
			info->Status = 127;
			PrintError(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void ForkCmdFunction(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, GetEnviron(info)) == -1)
		{
			FreeInfo(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->Status));
		if (WIFEXITED(info->Status))
		{
			info->Status = WEXITSTATUS(info->Status);
			if (info->Status == 126)
				PrintError(info, "Permission denied\n");
		}
	}
}
