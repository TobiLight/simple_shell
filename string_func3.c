/*
 * File: string_func3.c
 * Auth: Oluwatobiloba Light
 *       Princewill Chimdi Samuel
 */


#include "shellx.h"


int shellx_token_len(char *str, char *delim);
int shellx_token_count(char *str, char *delim);
char **shellx_strtok(char *str, char *delim);


/**
 * shellx_token_len - Set to locate
 * the delimiter i marking the end of the first
 * token contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The delimiter index marking the end of
 *         the intitial token pointed to be str.
 */
int shellx_token_len(char *str, char *delim)
{
	int f = 0, counts = 0;

	while (*(str + f) && *(str + f) != *delim)
	{
		counts++;
		f++;
	}

	return (counts);
}

/**
 * shellx_token_count - Set to count the number of
 * delimited words contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The number of words contained within str.
 */
int shellx_token_count(char *str, char *delim)
{
	int x, tokens = 0, counts = 0;

	for (x = 0; *(str + x); x++)
		counts++;

	for (x = 0; x < counts; x++)
	{
		if (*(str + x) != *delim)
		{
			tokens++;
			x += shellx_token_len(str + x, delim);
		}
	}

	return (tokens);
}

/**
 * shellx_strtok - Set to parses a string into a sequence of tokens
 * @str: Pointer to the string to be parsed
 * @delim: Pointer to bytes that delimits tokens in the parsed string
 *
 * Return: Pointer to the next token or NULL if no tokens
 */

char **shellx_strtok(char *str, char *delim)
{
	char **ptr;
	int o = 0, tokens, h, letters, l;

	tokens = shellx_token_count(str, delim);
	if (tokens == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (tokens + 2));
	if (ptr == NULL)
		return (NULL);

	for (h = 0; h < tokens; h++)
	{
		while (str[o] == *delim)
			o++;

		letters = shellx_token_len(str + o, delim);
		ptr[h] = malloc(sizeof(char) * (letters + 1));
		if (!ptr[h])
		{
			for (o -= 1; o >= 0; o--)
				free(ptr[o]);
			free(ptr);
			return (NULL);
		}

		for (l = 0; l < letters; l++)
		{
			ptr[h][l] = str[o];
			o++;
		}

		ptr[h][l] = '\0';
	}
	ptr[h] = NULL;
	ptr[h + 1] = NULL;

	return (ptr);
}
