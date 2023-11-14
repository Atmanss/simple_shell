#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **GetEnviron(info_t *info)
{
	if (!info->environ || info->Env_Chang)
	{
		info->environ = ListToStrings(info->env);
		info->Env_Chang = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int UnsetEnv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = StartsWithFunction(node->str, var);
		if (p && *p == '=')
		{
			info->Env_Chang = DeleteNodeAtIndex(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->Env_Chang);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int SetEnv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(StrLenFunction(var) + StrLenFunction(value) + 2);
	if (!buf)
		return (1);
	StrCpyFunction(buf, var);
	StrCatFunction(buf, "=");
	StrCatFunction(buf, value);
	node = info->env;
	while (node)
	{
		p = StartsWithFunction(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->Env_Chang = 1;
			return (0);
		}
		node = node->next;
	}
	AddNodeEnd(&(info->env), buf, 0);
	free(buf);
	info->Env_Chang = 1;
	return (0);
}
