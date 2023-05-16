/*
 * File: help_builtin2.c
 * Auth: Oluwatobiloba Light
 *       Princewill Chimdi Samuel
 */


#include "shellx.h"


/**
 * shellx_help_env - Set to displays information on the
 *            shellbx builtin command 'env'.
 *
 * Return: nothing
 */

void shellx_help_env(void)
{
	char *message = "env: env\n\tPrints the current environment.\n";

	write(STDOUT_FILENO, message, shellx_strlen(message));
}

/**
 * shellx_help_unsetenv - Set to displays information on
 *                 the shellbx builtin command
 * 'unsetenv'.
 *
 * Return: nothing
 */

void shellx_help_unsetenv(void)
{
	char *message = "unsetenv: unsetenv [VARIABLE]\n\tRemoves an ";

	write(STDOUT_FILENO, message, shellx_strlen(message));
	message = "environmental variable.\n\n\tUpon failure, prints a ";
	write(STDOUT_FILENO, message, shellx_strlen(message));
	message = "message to stderr.\n";
	write(STDOUT_FILENO, message, shellx_strlen(message));
}

/**
 * shellx_help_setenv - Set to displays information on
 *               the shellbx builtin command 'setenv'.
 *
 * Return: nothing
 */
void shellx_help_setenv(void)
{
	char *message = "setenv: setenv [VARIABLE] [VALUE]\n\tInitializes a new";

	write(STDOUT_FILENO, message, shellx_strlen(message));
	message = "environment variable, or modifies an existing one.\n\n";
	write(STDOUT_FILENO, message, shellx_strlen(message));
	message = "\tUpon failure, prints a message to stderr.\n";
	write(STDOUT_FILENO, message, shellx_strlen(message));
}
