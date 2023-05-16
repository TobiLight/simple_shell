/*
 * File: input_handlers.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */

#include "shellx.h"
char *shellx_get_args(char *, int *);
int shellx_call_args(char **, char **, int *);
int shellx_run_args(char **, char **, int *);
int shellx_handle_args(int *);
int shellx_check_args(char **);

/**
 * shellx_handle_args - Gets, calls, and runs the execution of a command.
 * @exec_ret: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int shellx_handle_args(int *exec_ret)
{
	int response = 0, i;
	char *line = NULL, **args, **beg_arg;

	line = shellx_get_args(line, exec_ret);
	if (line == NULL)
		return (SHELLX_END_OF_FILE);

	args = shellx_strtok(line, " ");
	free(line);
	if (args == NULL)
		return (response);
	if (shellx_check_args(args) != 0)
	{
		*exec_ret = 2;
		shellx_free_args(args, args);
		return (*exec_ret);
	}
	beg_arg = args;

	for (i = 0; args[i]; i++)
	{
		if (shellx_strncmp(args[i], ";", 1) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			response = shellx_call_args(args, beg_arg, exec_ret);
			args = &args[++i];
			i = 0;
		}
	}
	if (args)
		response = shellx_call_args(args, beg_arg, exec_ret);

	free(beg_arg);
	return (response);
}

/**
 * shellx_get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exec_ret: The return value of the last executed command.
 *
 * Return: If an error occurrs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *shellx_get_args(char *line, int *exec_ret)
{
	size_t size = 0;
	ssize_t read;
	char *prompt = "";

	if (line)
		free(line);

	read = shellx_getline(&line, &size, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		history++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (shellx_get_args(line, exec_ret));
	}

	line[read - 1] = '\0';
	shellx_replace_variable(&line, exec_ret);
	shellx_handle_line(&line, read);

	return (line);
}

/**
 * shellx_call_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @beg_arg: A double pointer to the beginning of args.
 * @exec_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int shellx_call_args(char **args, char **beg_arg, int *exec_ret)
{
	int response, i;

	if (args[0] == NULL)
		return (*exec_ret);
	for (i = 0; args[i]; i++)
	{
		if (shellx_strncmp(args[i], "||", 2) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			args = shellx_replace_aliases(args);
			response = shellx_run_args(args, beg_arg, exec_ret);
			if (*exec_ret != 0)
			{
				args = &args[++i];
				i = 0;
			}
			else
			{
				for (i++; args[i]; i++)
					free(args[i]);
				return (response);
			}
		}
		else if (shellx_strncmp(args[i], "&&", 2) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			args = shellx_replace_aliases(args);
			response = shellx_run_args(args, beg_arg, exec_ret);
			if (*exec_ret == 0)
			{
				args = &args[++i];
				i = 0;
			}
			else
			{
				for (i++; args[i]; i++)
					free(args[i]);
				return (response);
			}
		}
	}
	args = shellx_replace_aliases(args);
	response = shellx_run_args(args, beg_arg, exec_ret);
	return (response);
}

/**
 * shellx_run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @beg_arg: A double pointer to the beginning of args.
 * @exec_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int shellx_run_args(char **args, char **beg_arg, int *exec_ret)
{
	int response, i;
	int (*builtin)(char **args, char **beg_arg);

	builtin = shellx_get_builtin(args[0]);
	if (builtin)
	{
		response = builtin(args + 1, beg_arg);
		if (response != SHELLX_EXIT)
			*exec_ret = response;
	}
	else
	{
		*exec_ret = shellx_execute(args, beg_arg);
		response = *exec_ret;
	}
	history++;
	for (i = 0; args[i]; i++)
		if (args[i] != NULL)
			free(args[i]);

	return (response);
}

/**
 * shellx_check_args - Check for leading semicolons, ampersands, or pipes.
 *
 * @args: An array of arguments to check.
 *
 * Return: 0 if no invalid sequences are found, otherwise 2.
 */
int shellx_check_args(char **args)
{
	size_t i = 0;
	char *current, *next;

	while (args[i])
	{
		current = args[i];
		if (current[0] == ';' || current[0] == '&' || current[0] == '|')
		{
			if (i == 0 || current[1] == ';')
				return (shellx_create_error(&args[i], 2));
			next = args[i + 1];
			if (next && (next[0] == ';' || next[0] == '&' || next[0] == '|'))
				return (shellx_create_error(&args[i + 1], 2));
		}
		i++;
	}
	return (0);
}
