/*
 * File: loc.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */

#include "shellx.h"

char *shellx_fill_path_dir(char *);
sh_singly_list_t *shellx_get_path_dir(char *);

/**
 * shellx_get_loc - Locates a command in the PATH.
 * @cmd: The command to locate.
 *
 * Return: If an error occurs or the command cannot be located - NULL.
 *         Otherwise - the full pathname of the command.
 */
char *shellx_get_loc(char *cmd)
{
	char **path, *temp;
	sh_singly_list_t *directories, *head;
	struct stat st;

	path = shellx_getenv("PATH");
	if (path == NULL || (*path) == NULL)
		return (NULL);

	directories = shellx_get_path_dir(*path + 5);
	head = directories;
	while (directories)
	{
		temp = malloc(shellx_strlen(directories->dir) + shellx_strlen(cmd) + 2);
		if (!temp)
			return (NULL);

		shellx_strcpy(temp, directories->dir);
		shellx_strcat(temp, "/");
		shellx_strcat(temp, cmd);

		if (stat(temp, &st) == 0)
		{
			shellx_free_list(head);
			return (temp);
		}
		directories = directories->next;
		free(temp);
	}
	shellx_free_list(head);
	return (NULL);
}

/**
 * shellx_fill_path_dir - Copies path and also replaces
 *                        leading/sandwiched/trailing colons (:)
 *                        with current working directory.
 * @path: The colon-separated list of directories.
 *
 * Return: A copy of path with any leading/sandwiched/trailing
 *         colons replaced with the current working directory.
 */
char *shellx_fill_path_dir(char *path)
{
	int i, length = 0;
	char *path_cpy, *pwd;

	pwd = *(shellx_getenv("PWD")) + 4;
	i = 0;
	while (path[i])
	{
		if (path[i] == ':')
		{
			if (path[i + 1] == ':' || i == 0 || path[i + 1] == '\0')
				length += shellx_strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
		i++;
	}
	path_cpy = malloc(sizeof(char) * (length + 1));
	if (path_cpy == NULL)
		return (NULL);
	path_cpy[0] = '\0';
	i = 0;
	while (path[i])
	{
		if (path[i] == ':')
		{
			if (i == 0)
			{
				shellx_strcat(path_cpy, pwd);
				shellx_strcat(path_cpy, ":");
			}
			else if (path[i + 1] == ':' || path[i + 1] == '\0')
			{
				shellx_strcat(path_cpy, ":");
				shellx_strcat(path_cpy, pwd);
			}
			else
				shellx_strcat(path_cpy, ":");
		}
		else
		{
			shellx_strncat(path_cpy, &path[i], 1);
		}
		i++;
	}
	return (path_cpy);
}

/**
 * shellx_get_path_dir - Tokenizes a colon-separated list of
 *                directories into a sh_singly_list_T linked list.
 * @path: The colon-separated list of directories.
 *
 * Return: A pointer to the initialized linked list.
 */
sh_singly_list_t *shellx_get_path_dir(char *path)
{
	int i;
	char **directories, *path_cpy;
	sh_singly_list_t *head = NULL;

	path_cpy = shellx_fill_path_dir(path);
	if (path_cpy == NULL)
		return (NULL);
	directories = shellx_strtok(path_cpy, ":");
	free(path_cpy);
	if (!directories)
		return (NULL);
	i = 0;
	while (directories[i])
	{
		if (shellx_add_node_end(&head, directories[i]) == NULL)
		{
			shellx_free_list(head);
			free(directories);
			return (NULL);
		}
		i++;
	}
	free(directories);
	return (head);
}
