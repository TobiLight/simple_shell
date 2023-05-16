/*
 * File: helpers2.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */


#include "shellx.h"


ssize_t shellx_get_new_len(char *);
void shellx_logical_ops(char *, ssize_t *);
void shellx_handle_line(char **, ssize_t);


/**
 * shellx_logical_ops - Set to checks a line
 * for logical operators "||" or "&&".
 * @line: A pointer to the character to check in the line.
 * @new_len: Pointer to new_len in shellx_get_new_len function.
 *
 * Return: nothing
 */

void shellx_logical_ops(char *line, ssize_t *new_len)
{
	char prev1 = *(line - 1);
	char curr2 = *line;
	char next3 = *(line + 1);

	if (curr2 == '&')
	{
		if (next3 == '&' && prev1 != ' ')
			(*new_len)++;
		else if (prev1 == '&' && next3 != ' ')
			(*new_len)++;
	}
	else if (curr2 == '|')
	{
		if (next3 == '|' && prev1 != ' ')
			(*new_len)++;
		else if (prev1 == '|' && next3 != ' ')
			(*new_len)++;
	}
}

/**
 * shellx_handle_line - Set to parses
 * input line and adds necessary spaces
 * @line: Pointer to input line
 * @read: Length of input line
 *
 * Description: This function inserts spaces to separate ";", "||", and "&&",
 *              and replaces "#" with '\0'
 * Return: nothing
 */

void shellx_handle_line(char **line, ssize_t read)
{
	char *old_line, *new_line;
	char prev1, curr2, next3;
	size_t k, v;
	ssize_t new_len;

	new_len = shellx_get_new_len(*line);
	if (new_len == read - 1)
		return;
	new_line = malloc(new_len + 1);
	if (!new_line)
		return;
	k = 0, v = 0;
	old_line = *line;
	while (old_line[k])
	{
		curr2 = old_line[k];
		next3 = old_line[k + 1];
		if (k != 0)
		{
			prev1 = old_line[k - 1];
			if (curr2 == ';')
			{
				if (next3 == ';' && prev1 != ' ' && prev1 != ';')
				{
					new_line[v++] = ' ';
					new_line[v++] = ';';
					continue;
				}
				else if (prev1 == ';' && next3 != ' ')
				{
					new_line[v++] = ';';
					new_line[v++] = ' ';
					continue;
				}
				if (prev1 != ' ')
					new_line[v++] = ' ';
				new_line[v++] = ';';
				if (next3 != ' ')
					new_line[v++] = ' ';
				continue;
			}
			else if (curr2 == '&')
			{
				if (next3 == '&' && prev1 != ' ')
					new_line[v++] = ' ';
				else if (prev1 == '&' && next3 != ' ')
				{
					new_line[v++] = '&';
					new_line[v++] = ' ';
					continue;
				}
			}
			else if (curr2 == '|')
			{
				if (next3 == '|' && prev1 != ' ')
					new_line[v++] = ' ';
				else if (prev1 == '|' && next3 != ' ')
				{
					new_line[v++] = '|';
					new_line[v++] = ' ';
					continue;
				}
			}
		}
		else if (curr2 == ';')
		{
			if (k != 0 && old_line[k - 1] != ' ')
				new_line[v++] = ' ';
			new_line[v++] = ';';
			if (next3 != ' ' && next3 != ';')
				new_line[v++] = ' ';
			continue;
		}
		new_line[v++] = old_line[k];
		k++;
	}
	new_line[v] = '\0';
	free(*line);
	*line = new_line;
}

/**
 * shellx_get_new_len - Set to computes the
 * new length of a line partitioned
 *               by ";", "||", "&&" or "#".
 * @line: A pointer to the line to check.
 *
 * Description: Shortens lines containing '#' comments with '\0'.
 *
 * Return: The new length of the line.
 */

ssize_t shellx_get_new_len(char *line)
{
	size_t o = 0;
	ssize_t new_len = 0;
	char curr1, next2;

	while (line[o])
	{
		curr1 = line[o];
		next2 = line[o + 1];
		if (curr1 == '#')
		{
			if (o == 0 || line[o - 1] == ' ')
			{
				line[o] = '\0';
				break;
			}
		}
		else if (o != 0)
		{
			if (curr1 == ';')
			{
				if (next2 == ';' && line[o - 1] != ' ' && line[o - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (line[o - 1] == ';' && next2 != ' ')
				{
					new_len += 2;
					continue;
				}
				if (line[o - 1] != ' ')
					new_len++;
				if (next2 != ' ')
					new_len++;
			}
			else
				shellx_logical_ops(&line[o], &new_len);
		}
		else if (curr1 == ';')
		{
			if (o != 0 && line[o - 1] != ' ')
				new_len++;
			if (next2 != ' ' && next2 != ';')
				new_len++;
		}
		new_len++;
		o++;
	}
	return (new_len);
}
