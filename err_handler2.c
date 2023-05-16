/*
 * File: err_handler2.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */


#include "shellx.h"


/**
 * shellx_error_126 - Set to creates an error
 * message for permission denied failures.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */

char *shellx_error_126(char **args)
{
	char *error, *history_str;
	int count2;

	history_str = shellx_itoa(history);
	if (history_str == NULL)
		return (NULL);

	count2 = shellx_strlen(name) + shellx_strlen(history_str)
		+ shellx_strlen(args[0]) + 24;
	error = malloc(sizeof(char) * (count2 + 1));
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
	shellx_strcat(error, ": Permission denied\n");

	free(history_str);
	return (error);
}

/**
 * shellx_error_127 - Set to creates an error
 * message for command not found failures.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *shellx_error_127(char **args)
{
	char *error, *history_str;
	int count1;

	history_str = shellx_itoa(history);
	if (history_str == NULL)
		return (NULL);

	count1 = shellx_strlen(name) + shellx_strlen(history_str)
		+ shellx_strlen(args[0]) + 16;
	error = malloc(sizeof(char) * (count1 + 1));
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
	shellx_strcat(error, ": not found\n");

	free(history_str);
	return (error);
}
