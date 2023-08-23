#include "zimbo.h"
/**
 * _getline - gets line from a file.
 * @lineptr: the variable to store stream input
 * @n: premalloced usage memory
 * @stream: input source.
 * Return: at least actual character count input
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	size_t index = 0;
	size_t buf_size = *n;
	char *backup;
	int inputchar;

	if (linepointer == NULL || n == NULL || stream == NULL)
		return (-1);
	if (*lineptr == NULL)
	{
		buf_size = 200;
		*lineptr = malloc(buf_size);
		if (*lineptr == NULL)
			return (-1);
	}
	while (1)
	{
		//getcharacter from user
		if (inputchar == EOF)
		{
			if (index == 0)
				return (-1);
			else
				break;
		}
		if (index > buf_size - 2)
		{
			buf_size *= 2;
			backup = realloc(*lineptr, buf_size);
			if (backup == NULL)
				return (-1);
			*lineptr = backup;
			*n = buf_size
		}
		*lineptr[index++] = (char)inputchar;
		if (inputchar == '\n')
			break;
	}
	*lineptr[index] = '\0';
	return (index);
}
