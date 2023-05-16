/*
 * File: string_func1.c
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 */


#include "shellx.h"


char *shellx_strncat(char *dest, const char *src, size_t n);
char *shellx_strcat(char *dest, const char *src);
char *shellx_strchr(char *str, char c);
int shellx_strlen(const char *str);
int shellx_strncmp(const char *s1, const char *s2, size_t n);

/**
 * shellx_strlen - Set to returns length of a string
 * @str: Pointer to the string
 *
 * Return: The length of the string
 */

int shellx_strlen(const char *str)
{
	int count1 = 0;

	if (!str)
		return (count1);

	while (str[count1])
		count1++;

	return (count1);
}

/**
 * shellx_strncat - Set to concantenate two strings where n number
 *            of bytes are copied from the src
 * @dest: A pointer to the destination string
 * @src: A pointer to the source string
 * @n: n bytes to copy from src
 *
 * Return: A pointer to the destination string
 */


char *shellx_strncat(char *dest, const char *src, size_t n)
{
	size_t b;
	size_t len_of_dest = shellx_strlen(dest);

	for (b = 0; b < n && src[b] != '\0'; b++)
		dest[len_of_dest + b] = src[b];

	dest[len_of_dest + b] = '\0';
	return (dest);
}

/**
 * shellx_strcat - Set to concantenate two strings where n
 *  number of bytes are copied from the src
 * @dest: A pointer to the destination string
 * @src: A pointer to the source string
 *
 * Return: A pointer to the destination string
 */

char *shellx_strcat(char *dest, const char *src)
{
	char *destTemp;
	const char *srcTemp;

	destTemp = dest;
	srcTemp = src;

	while (*destTemp != '\0')
		destTemp++;

	while (*srcTemp != '\0')
		*destTemp++ = *srcTemp++;
	*destTemp = '\0';
	return (dest);
}

/**
 * shellx_strchr - Set to locate character in string
 * @str: The string to search
 * @c: The character to search for
 *
 * Return: Pointer to the first occurrence of the character c in
 *         the string str, or NULL if the character is not found
 */

char *shellx_strchr(char *str, char c)
{
	while (*str != '\0')
	{
		if (*str == c)
			return (str);
		str++;
	}

	return (NULL);
}

/**
 * shellx_strncmp - Set to compare two strings up to a given number of bytes
 * @s1: The first string
 * @s2: The second string
 * @n: The maximum number of bytes to compare
 *
 * Return: An integer less than, equal to, or greater than zero if the first n
 *         bytes of s1 is found, respectively, to be less than, to match, or be
 *         greater than the first n bytes of s2
 */
int shellx_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t b;

	for (b = 0; s1[b] && s2[b] && b < n; b++)
	{
		if (s1[b] > s2[b])
			return (s1[b] - s2[b]);
		else if (s1[b] < s2[b])
			return (s1[b] - s2[b]);
	}
	if (b == n)
		return (0);
	else
		return (-15);
}
