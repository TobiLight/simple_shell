/*
 * File: err_handler2.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */


#include "shellx.h"


int shellx_create_error(char **args, int err);
int shellx_num_len(int num);

/**
 * shellx_num_len - Set to counts the
 * digit length of a number.
 * @num: The number to measure.
 *
 * Return: The digit length.
 */
int shellx_num_len(int num)
{
	unsigned int num1;
	int count1 = 1;

	if (num < 0)
	{
		count1++;
		num1 = num * -1;
	}
	else
	{
		num1 = num;
	}
	while (num1 > 9)
	{
		count1++;
		num1 /= 10;
	}

	return (count1);
}

/**
 * shellx_itoa - Set to converts an integer to a string.
 * @num: The integer.
 *
 * Return: The converted string
 */
char *shellx_itoa(int num)
{
	char *buffer;
	int count2 = shellx_num_len(num);
	unsigned int num1;

	buffer = malloc(sizeof(char) * (count2 + 1));
	if (!buffer)
		return (NULL);

	buffer[count2] = '\0';

	if (num < 0)
	{
		num1 = num * -1;
		buffer[0] = '-';
	}
	else
	{
		num1 = num;
	}

	count2--;
	do {
		buffer[count2] = (num1 % 10) + '0';
		num1 /= 10;
		count2--;
	} while (num1 > 0);

	return (buffer);
}

/**
 * shellx_create_error - Set to write a
 * custom error message to stderr.
 * @args: An array of arguments.
 * @err: The error value.
 *
 * Return: The error value.
 */

int shellx_create_error(char **args, int err)
{
	char *error = NULL;

	switch (err)
	{
	case -1:
		error = shellx_error_env(args);
		break;
	case 1:
		error = shellx_error_1(args);
		break;
	case 2:
		if (*(args[0]) == 'e')
			error = shellx_error_2_exit(++args);
		else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
			error = shellx_error_2_syntax(args);
		else
			error = shellx_error_2_cd(args);
		break;
	case 126:
		error = shellx_error_126(args);
		break;
	case 127:
		error = shellx_error_127(args);
		break;
	default:
		break;
	}
	write(STDERR_FILENO, error, shellx_strlen(error));
	if (error)
		free(error);
	return (err);
}
