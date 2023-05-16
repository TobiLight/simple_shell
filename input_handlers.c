/*
 * File: input_handlers.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */

#include "shell.h"
char *shelly_get_args(char *, int *);
int shelly_call_args(char **, char **, int *);
int shelly_run_args(char **, char **, int *);
int shelly_handle_args(int *);
int shelly_check_args(char **);

/**
 * shelly_handle_args - Gets, calls, and runs the execution of a command.
 * @exec_ret: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int shelly_handle_args(int *exec_ret)
{
	int response = 0, i;
	char *line = NULL, **args, **beg_arg;

	line = shelly_get_args(line, exec_ret);
	if (line == NULL)
		return (SHELLY_END_OF_FILE);

	args = shelly_strtok(line, " ");
	free(line);
	if (args == NULL)
		return (response);
	if (shelly_check_args(args) != 0)
	{
		*exec_ret = 2;
		shelly_free_args(args, args);
		return (*exec_ret);
	}
	beg_arg = args;

	for (i = 0; args[i]; i++)
	{
		if (shelly_strncmp(args[i], ";", 1) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			response = shelly_call_args(args, beg_arg, exec_ret);
			args = &args[++i];
			i = 0;
		}
	}
	if (args)
		response = shelly_call_args(args, beg_arg, exec_ret);

	free(beg_arg);
	return (response);
}

/**
 * shelly_get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exec_ret: The return value of the last executed command.
 *
 * Return: If an error occurrs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *shelly_get_args(char *line, int *exec_ret)
{
	size_t size = 0;
	ssize_t read;
	char *prompt = "";

	if (line)
		free(line);

	read = shelly_getline(&line, &size, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		history++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (shelly_get_args(line, exec_ret));
	}

	line[read - 1] = '\0';
	shelly_replace_variable(&line, exec_ret);
	shelly_handle_line(&line, read);

	return (line);
}

/**
 * shelly_call_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @beg_arg: A double pointer to the beginning of args.
 * @exec_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int shelly_call_args(char **args, char **beg_arg, int *exec_ret)
{
	int response, i;

	if (args[0] == NULL)
		return (*exec_ret);
	for (i = 0; args[i]; i++)
	{
		if (shelly_strncmp(args[i], "||", 2) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			args = shelly_replace_aliases(args);
			response = shelly_run_args(args, beg_arg, exec_ret);
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
		else if (shelly_strncmp(args[i], "&&", 2) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			args = shelly_replace_aliases(args);
			response = shelly_run_args(args, beg_arg, exec_ret);
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
	args = shelly_replace_aliases(args);
	response = shelly_run_args(args, beg_arg, exec_ret);
	return (response);
}

/**
 * shelly_run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @beg_arg: A double pointer to the beginning of args.
 * @exec_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int shelly_run_args(char **args, char **beg_arg, int *exec_ret)
{
	int response, i;
	int (*builtin)(char **args, char **beg_arg);

	builtin = shelly_get_builtin(args[0]);
	if (builtin)
	{
		response = builtin(args + 1, beg_arg);
		if (response != SHELLY_EXIT)
			*exec_ret = response;
	}
	else
	{
		*exec_ret = shelly_execute(args, beg_arg);
		response = *exec_ret;
	}
	history++;
	for (i = 0; args[i]; i++)
		if (args[i] != NULL)
			free(args[i]);

	return (response);
}

/**
 * shelly_check_args - Check for leading semicolons, ampersands, or pipes.
 *
 * @args: An array of arguments to check.
 *
 * Return: 0 if no invalid sequences are found, otherwise 2.
 */
int shelly_check_args(char **args)
{
	size_t i = 0;
	char *current, *next;

	while (args[i])
	{
		current = args[i];
		if (current[0] == ';' || current[0] == '&' || current[0] == '|')
		{
			if (i == 0 || current[1] == ';')
				return (shelly_create_error(&args[i], 2));
			next = args[i + 1];
			if (next && (next[0] == ';' || next[0] == '&' || next[0] == '|'))
				return (shelly_create_error(&args[i + 1], 2));
		}
		i++;
	}
	return (0);
}
