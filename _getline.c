/*
 * File: _getline.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */

#include "shellx.h"

void shellx_appoint_lineptr(char **, size_t *, char *, size_t);

/**
 * shellx_getline - Reads input from a stream.
 * @lineptr: A buffer to store the input.
 * @n: The size of lineptr.
 * @stream: The stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t shellx_getline(char **lineptr, size_t *n, FILE *stream)
{
// 	static ssize_t input;
// 	ssize_t ret;
// 	char *buffer, x = 'x';
// 	int r;

// 	if (input == 0)
// 		fflush(stream);
// 	else
// 		return (-1);
// 	input = 0;
// 	buffer = malloc(sizeof(char) * 120);
// 	if (buffer == NULL)
// 		return (-1);

// 	while (x != '\n')
// 	{
// 		r = read(STDIN_FILENO, &x, 1);
// 		if (r == -1 || (r == 0 && input == 0))
// 		{
// 			free(buffer);
// 			return (-1);
// 		}
// 		if (r == 0 && input != 0)
// 		{
// 			input++;
// 			break;
// 		}

// 		if (input >= 120)
// 			buffer = shellx_realloc(buffer, input, input + 1);

// 		buffer[input] = x;
// 		input++;
// 	}
// 	buffer[input] = '\0';
// 	shellx_appoint_lineptr(lineptr, n, buffer, input);
// 	ret = input;
// 	if (r != 0)
// 		input = 0;
// 	return (ret);
	
	static char buffer[120];
	static ssize_t input = 0;
	ssize_t ret;
	char x = 'x';

	if (input == 0)
	{
		fflush(stream);
		input = read(STDIN_FILENO, buffer, BUFFER_SIZE);
		if (input <= 0)
		{
			return (-1);
		}
	}
	else
	{
		return (-1);
	}

	for (; input > 0; input--)
	{
		x = buffer[input - 1];
		if (x == '\n' || x == '\r')
		{
			break;
		}
	}
	
	*lineptr = realloc(*lineptr, input + 1);
	if (*lineptr == NULL)
	{
		return (-1);
	}
	
	for (ssize_t i = 0; i < input; i++)
	{
		(*lineptr)[i] = buffer[i];
	}
	(*lineptr)[input] = '\0';
	ret = input;
	
	if (x == '\n' || x == '\r')
	{
		input = 0;
	}
	
	shellx_appoint_lineptr(lineptr, n, *lineptr, input);
	return (ret);
}

/**
 * shellx_appoint_lineptr - Reassigns the lineptr variable for shellx_getline.
 * @lineptr: A buffer to store an input string.
 * @n: The size of lineptr.
 * @buffer: The string to assign to lineptr.
 * @b: The size of buffer.
 *
 * Return: void.
 */
void shellx_appoint_lineptr(char **lineptr, size_t *n, char *buffer, size_t b)
{
	if (*lineptr == NULL)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else
	{
		shellx_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * shellx_realloc - Reallocates a memory block using malloc and free.
 * @ptr: A pointer to the memory previously allocated.
 * @old_size: The size in bytes of the allocated space for ptr.
 * @new_size: The size in bytes for the new memory block.
 *
 * Return: If new_size == old_size - ptr.
 *         If new_size == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *shellx_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *memory;
	char *ptr_cpy, *fill;
	unsigned int i = 0;

	if (new_size == old_size)
		return (ptr);
	if (ptr == NULL)
	{
		memory = malloc(new_size);
		if (memory == NULL)
			return (NULL);
		return (memory);
	}
	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_cpy = ptr;
	memory = malloc(sizeof(*ptr_cpy) * new_size);
	if (memory == NULL)
	{
		free(ptr);
		return (NULL);
	}
	fill = memory;
	while (i < old_size && i < new_size)
	{
		fill[i] = *ptr_cpy++;
		i++;
	}
	free(ptr);
	return (memory);
}
