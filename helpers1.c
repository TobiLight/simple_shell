/*
 * File: helpers1.c
 * Authors: Oluwatobiloba Agunloye
 *          Princewill Chimdi Samuel
 */

#include "shellx.h"

char *shellx_get_pid(void);
char *shellx_get_env_value(char *, int);

/**
 * shellx_get_pid - Gets the current process ID.
 * Description: Opens the stat file, a space-delimited file containing
 *              information about the current process. The PID is the
 *              first word in the file. The function reads the PID into
 *              a buffer and replace the space at the end with a \0 byte.
 *
 * Return: The current process ID or NULL on failure.
 */
char *shellx_get_pid(void)
{
	size_t i = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}

/**
 * shellx_get_env_value - Gets the value corresponding
 *                        to an environmental variable.
 * @beginning: The environmental variable to search for.
 * @length: The length of the environmental variable to search for.
 *
 * Return: If the variable is not found - an empty string.
 *         Otherwise - the value of the environmental variable.
 *
 * Description: Variables are stored in the format VARIABLE=VALUE.
 */
char *shellx_get_env_value(char *beginning, int length)
{
	char **var_addy;
	char *repl = NULL, *temp, *var;

	var = malloc(length + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	shellx_strncat(var, beginning, length);

	var_addy = shellx_getenv(var);
	free(var);
	if (var_addy)
	{
		temp = *var_addy;
		while (*temp != '=')
			temp++;
		temp++;
		repl = malloc(shellx_strlen(temp) + 1);
		if (repl)
			shellx_strcpy(repl, temp);
	}

	return (repl);
}

/**
 * shellx_free_args - Frees up memory taken by args.
 * @args: A null-terminated double pointer containing commands/arguments.
 * @beg_args: A double pointer to the beginning of args.
 *
 * Return: nothing
 */
void shellx_free_args(char **args, char **beg_args)
{
	size_t i;

	for (i = 0; args[i] || args[i + 1]; i++)
		free(args[i]);

	free(beg_args);
}

/**
 * shellx_replace_variable - Replaces special strings in the command line.
 * @line: A double pointer containing the command and arguments.
 * @exe_ret: A pointer to the return value of the last executed command.
 *
 * Description: Replaces $$ with the current PID, $? with the return value
 *              of the last executed program, and environmental variables
 *              preceded by $ with their corresponding value.
 */
void shellx_replace_variable(char **line, int *exe_ret)
{
	int i, j = 0, len;
	char *repl = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (i = 0; old_line[i]; i++)
	{
		if (old_line[i] == '$' && old_line[i + 1] &&
			old_line[i + 1] != ' ')
		{
			if (old_line[i + 1] == '$')
			{
				repl = shellx_get_pid();
				j = i + 2;
			}
			else if (old_line[i + 1] == '?')
			{
				repl = shellx_itoa(*exe_ret);
				j = i + 2;
			}
			else if (old_line[i + 1])
			{
				/* extract the variable name to search for */
				for (j = i + 1; old_line[j] &&
								old_line[j] != '$' &&
								old_line[j] != ' ';
					 j++)
					;
				len = j - (i + 1);
				repl = shellx_get_env_value(&old_line[i + 1], len);
			}
			new_line = malloc(i + shellx_strlen(repl)
					+ shellx_strlen(&old_line[j]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			shellx_strncat(new_line, old_line, i);
			if (repl)
			{
				shellx_strcat(new_line, repl);
				free(repl);
				repl = NULL;
			}
			shellx_strcat(new_line, &old_line[j]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			i = -1;
		}
	}
}
