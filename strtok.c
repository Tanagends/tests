#include "zimbo.h"
/**
 * _strtok - tokenizes the string.
 * @str: the string to be tokenized.
 * @delimiter: the delimiter.
 * Return:
 */
char *_strtok(char *str, const char *delimiters)
{
	static char token[MAX_LINE] = "";
	int i, k, j = 0;
	char *result;

	if (str != NULL)
		token[0] = '\0';
	for (i = 0; str[i] != '\0'; i++)
	{
		for(k = 0; delimiter[k] != '\0'; k++)
		{
			if (str[i] != delimiter[k])
				continue;
			else
				break;
		}
		if (delimiter[k] == '\0')
		{
			if (token == '\0')
				token[j++] = str[i];
			else
				strcat(token, (char *)str[i]);
		}
		else
			break;
	}
	token[j] = '\0';
	if (j == 0 && str[i] == '\0')
		return (NULL);
	result = malloc(strlen(token) + 1);
	strcpy(result, token);
	return (token);
}
