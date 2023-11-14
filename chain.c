#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int IsChain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->CMD_Buf_type = CommandOr;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->CMD_Buf_type = CommandAnd;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->CMD_Buf_type = CommandChain;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last Status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void CheckChain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->CMD_Buf_type == CommandAnd)
	{
		if (info->Status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->CMD_Buf_type == CommandOr)
	{
		if (!info->Status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int ReplaceAlias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = NodeStartsWith(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = StrDupFunction(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int ReplaceVariables(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!StrCmpFunction(info->argv[i], "$?"))
		{
			ReplaceString(&(info->argv[i]),
				StrDupFunction(ConvertNumber(info->Status, 10, 0)));
			continue;
		}
		if (!StrCmpFunction(info->argv[i], "$$"))
		{
			ReplaceString(&(info->argv[i]),
				StrDupFunction(ConvertNumber(getpid(), 10, 0)));
			continue;
		}
		node = NodeStartsWith(info->env, &info->argv[i][1], '=');
		if (node)
		{
			ReplaceString(&(info->argv[i]),
				StrDupFunction(_strchr(node->str, '=') + 1));
			continue;
		}
		ReplaceString(&info->argv[i], StrDupFunction(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int ReplaceString(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
