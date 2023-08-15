#include "zimbo.h"
char *builtstr = {"cd", "setenv", "env", "exit"};
int (*builtfunc[])(char **toks) = {zimbo_cd, zimbo_setenv, zimbo_env, 
	zimbo_exit};

int main(void)
{
	char *input, **toks;
	size_t size = 0, i;

	ssize_t i = write(1, "Zimboshell$", 11);
	if (i != -1)
	{
		ssize_t k = getline(&input, &size, stdin);
		if (k != -1)
		{
			toks = zimbo_split(input);
	}
	return 0;
}
/**
 * zimbo_split - tokenize string.
 * @string: string to be tokenized.
 *
 * Return: tokenized string.
 */
char **zimbo_split(char *string)
{
	char *tok;
	size_t count = 0;
	char **toks = malloc(MAX_LINE * sizeof(char *));

	if (!toks)
	{
		write(1, "Malloc error", 11);
		exit(EXIT_FAILURE);
	}
	tok = strtok(string, ZIMBO_DELIM);
	while (tok != NULL)
	{
		toks[count++] = tok;
		tok = strtok(NULL, ZIMBO_DELIM);
	}
	toks[count] = NULL;
	return (toks);
}
