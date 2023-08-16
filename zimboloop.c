#include "zimbo.h"
//char *builtstr = {"cd", "setenv", "env", "exit"};
//int (*builtfunc[])(char **toks) = {zimbo_cd, zimbo_setenv, zimbo_env, 
//	zimbo_exit};
extern char** environ;
int main(void)
{
	char *input, **toks;
	size_t size = 0, j = 0;
	ssize_t i = write(1, "Zimboshell$", 11);
	ssize_t k;
	int status;

	if (i != -1)
	{
		k = getline(&input, &size, stdin);
		if (k != -1)
		{
			toks = zimbo_split(input);
		}
		else
			printf("Error\n");
	}
	status = zimbo_execute(toks);
	free(toks);
	free(input);
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
/**
 * zimbo_execute - executes commands
 * 
 */
int zimbo_execute(char **toks)
{
	pid_t _pid;
	int status;

	_pid = fork();
	if (toks == NULL)
		return (1);
	//Child process.
	if (_pid == 0)
	{
		if (execve(toks[0], toks, environ) == -1)
		{
			perror("No such file or directory");
			exit(EXIT_FAILURE);
		}
	}
	else if (_pid > 0)
            // Parent process
		waitpid(_pid, &status, WUNTRACED); // Wait for the child process to finish.
	else
		perror("fork error"); // If fork fails
	return (1);
}
