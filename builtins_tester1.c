#include "shell.h"

/**
 * _myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit Status
 *         (0) if info.argv[0] != "exit"
 */
int MyExit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = ErrorAtoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->Status = 2;
			PrintError(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = ErrorAtoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int MyCd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		PutsFunction("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = GetEnv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = GetEnv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (StrCmpFunction(info->argv[1], "-") == 0)
	{
		if (!GetEnv(info, "OLDPWD="))
		{
			PutsFunction(s);
			_putchar('\n');
			return (1);
		}
		PutsFunction(GetEnv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = GetEnv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		PrintError(info, "can't cd to ");
		_eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		SetEnv(info, "OLDPWD", GetEnv(info, "PWD="));
		SetEnv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int MyHelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	PutsFunction("help call works. Function not yet implemented \n");
	if (0)
		PutsFunction(*arg_array); /* temp att_unused workaround */
	return (0);
}
