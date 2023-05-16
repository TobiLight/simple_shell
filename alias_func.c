/*
 * File: alias_func.c
 * Authors: Oluwatobiloba Light
 *	    Princewill Chimdi Samuel
*/


#include "shellx.h"

void shellx_set_alias(char *, char *);
void shellx_print_alias(sh_alias_t *);


/**
 * shellx_alias - Builtin command that either prints all aliases, specific
 *                aliases, or sets an alias.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellx_alias(char **args, char __attribute__((__unused__)) **front)
{

	sh_alias_t *temp = aliases;
	int f, response = 0;
	char *value;

	if (args[0] == NULL)
	{
		while (temp)
		{
			shellx_print_alias(temp);
			temp = temp->next;
		}
		return (response);
	}

	for (f = 0; args[f]; f++)
	{
		temp = aliases;
		value = shellx_strchr(args[f], '=');

		if (value == NULL)
		{
			while (temp)
			{
				if (shellx_strcmp(args[f], temp->name) == 0)
				{
					shellx_print_alias(temp);
					break;
				}
				temp = temp->next;
			}

			if (temp == NULL)
				response = shellx_create_error(args + f, 1);
		}
		else
		{
			shellx_set_alias(args[f], value);
		}
	}
	return (response);
}

/**
 * shellx_set_alias - Set an alias with the given name and value,
 *                    or modify an existing alias with the new value.
 * @var_name: Name of the alias.
 * @value: Value of the alias. First character is a '='.
 */
void shellx_set_alias(char *var_name, char *value)
{
	sh_alias_t *temp = aliases;
	char *new_value;
	int len, d, u;

	*value = '\0';
	value++; /* skip the '=' character */
	len = shellx_strlen(value) - shellx_strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (new_value == NULL)
		return;

	for (d = 0, u = 0; value[d]; d++)
	{
		if (value[d] != '\'' && value[d] != '"')
			new_value[u++] = value[d];
	}

	new_value[u] = '\0';
	while (temp)
	{
		if (shellx_strcmp(var_name, temp->name) == 0)
		{
			free(temp->val);
			temp->val = new_value;
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL)
		shellx_add_alias_end(&aliases, var_name, new_value);
}

/**
 * shellx_print_alias - Prints an alias in the format "name='value'\n".
 *
 * @alias: The alias to print.
 */
void shellx_print_alias(sh_alias_t *alias)
{

	char *alias_string;
	int count;

	count = shellx_strlen(alias->name) + shellx_strlen(alias->val) + 4;
	alias_string = malloc(count + 1);
	if (alias_string == NULL)
		return;

	shellx_strcpy(alias_string, alias->name);
	shellx_strcat(alias_string, "='");
	shellx_strcat(alias_string, alias->val);
	shellx_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, count);
	free(alias_string);
}

/**
 * shellx_replace_aliases - Replaces any matching alias in
 *                          the arguments with its value.
 * @args: Double pointer to the arguments.
 *
 * Return: Double pointer to the arguments.
 */
char **shellx_replace_aliases(char **args)
{
	sh_alias_t *temp;
	int r;
	char *new_value;

	if (shellx_strcmp(args[0], "alias") == 0)
		return (args);
	r = 0;
	while (args[r] != NULL)
	{
		temp = aliases;
		while (temp != NULL)
		{
			if (shellx_strcmp(args[r], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (shellx_strlen(temp->val) + 1));
				if (new_value == NULL)
				{
					shellx_free_args(args, args);
					return (NULL);
				}
				shellx_strcpy(new_value, temp->val);
				free(args[r]);
				args[r] = new_value;
				r--;
				break;
			}
			temp = temp->next;
		}
		r++;
	}
	return (args);
}
