/*
 * File: builtin.c
 * Auth: Oluwatobiloba Light
 *       Princewill Chimdi Samuel
 */

#include "shellx.h"

int shellx_exit(char **, char **);
int shellx_cd(char **, char __attribute__((__unused__)) **);
int shellx_help(char **, char __attribute__((__unused__)) **);
int (*get_builtin(char *))(char **, char **);

/**
 * shellx_exit - Exits the shell.
 * @args: An array of arguments.
 * @front_arg: A double pointer to the beginning of args.
 *
 * Return: If there are no arguments - -3.
 *         If the given exit value is invalid - 2.
 *         O/w - exits with the given status value.
 */
int shellx_exit(char **args, char **front_arg)
{
	int i = 0, integer_length = 10;
	unsigned int max = 1 << (sizeof(int) * 8 - 1), num = 0;

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			integer_length++;
		}
		while (args[0][i])
		{
			if (i <= integer_length && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (shellx_create_error(--args, 2));
			i++;
		}
	}
	else
	{
		return (-3);
	}

	if (num > max - 1)
		return (shellx_create_error(--args, 2));

	args -= 1;
	shellx_free_args(args, front_arg);
	shellx_free_env();
	shellx_free_alias_list(aliases);
	exit(num);
}

/**
 * shellx_cd - Changes the current directory of the shellby process.
 * @args: An array of arguments.
 * @front_arg: A double pointer to the beginning of args.
 *
 * Return: If the given string is not a directory - 2.
 *         If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellx_cd(char **args, char __attribute__((__unused__)) **front_arg)
{
	char **directory_info, *new_line = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat directory;

	oldpwd = getcwd(oldpwd, 0);
	if (oldpwd == NULL)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || shellx_strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
				args[0][1] == '\0')
			{
				if (shellx_getenv("OLDPWD") != NULL)
					(chdir(*shellx_getenv("OLDPWD") + 7));
			}
			else
			{
				free(oldpwd);
				return (shellx_create_error(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode) &&
				((dir.st_mode & S_IXUSR) != 0))
			{
				chdir(args[0]);
			}
			else
			{
				free(oldpwd);
				return (shellx_create_error(args, 2));
			}
		}
	}
	else
	{
		if (shellx_getenv("HOME") != NULL)
			chdir(*(shellx_getenv("HOME")) + 5);
	}
	pwd = getcwd(pwd, 0);
	if (pwd == NULL)
	{
		free(oldpwd);
		return (-1);
	}

	directory = malloc(sizeof(char *) * 2);
	if (directory == NULL)
	{
		free(oldpwd);
		free(pwd);
		return (-1);
	}

	directory[0] = "OLDPWD";
	directory[1] = oldpwd;
	if (shellx_setenv(directory, directory) == -1)
	{
		free(oldpwd);
		free(pwd);
		free(directory);
		return (-1);
	}

	directory[0] = "PWD";
	directory[1] = pwd;
	if (shellx_setenv(directory, directory) == -1)
	{
		free(oldpwd);
		free(pwd);
		free(directory);
		return (-1);
	}
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, shellx_strlen(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(oldpwd);
	free(pwd);
	free(directory_info);
	return (0);
}

/**
 * shellx_help - Displays information about shellby builtin commands.
 * @args: An array of arguments.
 * @front_arg: A pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on failure.
 */
int shellx_help(char **args, char __attribute__((__unused__)) **front_arg)
{
	if (args[0] == NULL)
		shellx_help_all();
	else if (shellx_strcmp(args[0], "help") == 0)
		shellx_help_help();
	else if (shellx_strcmp(args[0], "cd") == 0)
		shellx_help_cd();
	else if (shellx_strcmp(args[0], "alias") == 0)
		shellx_help_alias();
	else if (shellx_strcmp(args[0], "exit") == 0)
		shellx_help_exit();
	else if (shellx_strcmp(args[0], "env") == 0)
		shellx_help_env();
	else if (shellx_strcmp(args[0], "setenv") == 0)
		shellx_help_setenv();
	else if (shellx_strcmp(args[0], "unsetenv") == 0)
		shellx_help_unsetenv();
	else
		write(STDERR_FILENO, name, shellx_strlen(name));

	return (0);
}

/**
 * shellx_get_builtin - Matches a command with a corresponding
 *               shellx builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding builtin.
 */
int (*shellx_get_builtin(char *command))(char **args, char **front_arg)
{
	int x;
	sh_builtin_t bf[] = {
		{"exit", shellx_exit},
		{"env", shellx_env},
		{"setenv", shellx_setenv},
		{"unsetenv", shellx_unsetenv},
		{"cd", shellx_cd},
		{"alias", shellx_alias},
		{"help", shellx_help},
		{NULL, NULL}};

	x = 0;
	while (bf[x].name)
	{

		if (shellx_strcmp(bf[x].name, command) == 0)
			break;
		x++;
	}
	return (bf[x].func);
}
