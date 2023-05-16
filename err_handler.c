/*
 * File: err_handler.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */

#include "shellx.h"

/**
 * shellx_error_env - Creates an error message for shellby_env errors.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *shellx_error_env(char **args)
{
	char *error, *history_str;
	int len;

	history_str = shellx_itoa(history);
	if (history_str == NULL)
		return (NULL);

	args--;
	len = shellx_strlen(name) + shellx_strlen(history_str)
		+ shellx_strlen(args[0]) + 45;
	error = malloc(sizeof(char) * (len + 1));
	if (error == NULL)
	{
		free(history_str);
		return (NULL);
	}

	shellx_strcpy(error, name);
	shellx_strcat(error, ": ");
	shellx_strcat(error, history_str);
	shellx_strcat(error, ": ");
	shellx_strcat(error, args[0]);
	shellx_strcat(error, ": Unable to add/remove from environment\n");

	free(history_str);
	return (error);
}

/**
 * shellx_error_1 - Creates an error message for shellby_alias errors.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *shellx_error_1(char **args)
{
	char *error;
	int length = 0;

	length += shellx_strlen(name) + shellx_strlen(args[0]) + 13;
	error = malloc(sizeof(char) * (length + 1));
	if (error == NULL)
		return (NULL);

	shellx_strcpy(error, "alias: ");
	shellx_strcat(error, args[0]);
	shellx_strcat(error, " not found\n");

	return (error);
}

/**
 * shellx_error_2_exit - Creates an error message for shellby_exit errors.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *shellx_error_2_exit(char **args)
{
	char *error, *history_str;
	int len;

	history_str = shellx_itoa(history);
	if (history_str == NULL)
		return (NULL);

	len = shellx_strlen(name) + shellx_strlen(history_str)
		+ shellx_strlen(args[0]) + 27;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(history_str);
		return (NULL);
	}

	shellx_strcpy(error, name);
	shellx_strcat(error, ": ");
	shellx_strcat(error, history_str);
	shellx_strcat(error, ": exit: Illegal number: ");
	shellx_strcat(error, args[0]);
	shellx_strcat(error, "\n");

	free(history_str);
	return (error);
}

/**
 * shellx_error_2_cd - Creates an error message for cd errors.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *shellx_error_2_cd(char **args)
{
	char *error, *history_str;
	int len;

	history_str = shellx_itoa(history);
	if (history_str == NULL)
		return (NULL);

	len = shellx_strlen(name) + shellx_strlen(history_str)
		+ shellx_strlen(args[0]) + 25;
	if (args[0][0] == '-')
		len += 15;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(history_str);
		return (NULL);
	}

	shellx_strcpy(error, name);
	shellx_strcat(error, ": ");
	shellx_strcat(error, history_str);
	if (args[0][0] == '-')
		shellx_strcat(error, ": cd: illegal option -- ");
	else
		shellx_strcat(error, ": cd: can't cd to ");
	shellx_strcat(error, args[0]);
	shellx_strcat(error, "\n");

	free(history_str);
	return (error);
}

/**
 * shellx_error_2_syntax - Creates an error message for syntax errors.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *shellx_error_2_syntax(char **args)
{
	char *error, *history_str;
	int length;

	history_str = shellx_itoa(history);
	if (history_str == NULL)
		return (NULL);

	length = shellx_strlen(name) + shellx_strlen(history_str)
		+ shellx_strlen(args[0]) + 33;
	error = malloc(sizeof(char) * (length + 1));
	if (!error)
	{
		free(history_str);
		return (NULL);
	}

	shellx_strcpy(error, name);
	shellx_strcat(error, ": ");
	shellx_strcat(error, history_str);
	shellx_strcat(error, ": ");
	shellx_strcat(error, args[0]);
	shellx_strcat(error, ": Syntax error: \"");
	shellx_strcat(error, args[0]);
	shellx_strcat(error, "\" unexpected\n");

	free(history_str);
	return (error);
}
