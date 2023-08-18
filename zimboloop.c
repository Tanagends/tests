#include "zimbo.h"
/**
 * main - a simple shell
 * Return: 0(Always success).
 */
//char *builtstr = {"cd", "setenv", "env", "exit"};
//int (*builtfunc[])(char **toks) = {zimbo_cd, zimbo_setenv, zimbo_env, 
//	zimbo_exit};
// command prompt duplicating upon null input
// double free error
// error messages
extern char** environ;
int main(int argc, char **argv, char **envp)
{
	char *input, **toks;
	size_t size = 0;
	ssize_t i, k;
	int status;

	while (1)
	{
		write(1, "Zimboshell$ ", 12);
		if (i != -1)
		{
			k = getline(&input, &size, stdin);
			if (k != -1)
			{
				toks = zimbo_split(input);
			//else
			//	printf("Error\n");
			//	perror(toks[0]);
				status = zimbo_execute(toks);
				//free(input);
				free(toks);
			}
			else if (input == "\n")
				write(1, "\n", 1);
		}
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
		perror("Malloc Error");
		exit(errno);
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
	char *path_handler = NULL;

	if (toks[0] == NULL)
		return (1);
	//handle inbuilt
	path_handler = zimbo_path__handler(toks);//remember to free path handler
	if (path_handler != NULL || access(toks[0], X_OK) == 0)
	{
		//we will call path handler if command exists
		/*Child process.*/
		_pid = fork();
		if (_pid == 0)
		{
			if (path_handler != NULL)
			{
				if (execve(path_handler, toks, environ) == -1)
				{
					perror(toks[0]);
					free(path_handler);
					exit(errno);
				}
				free(path_handler);
			}
			else
			{
				if (execve(toks[0], toks, environ) == -1)
				{
					perror(toks[0]);
					exit(errno);
				}
			}
		}
		else if (_pid > 0)
		    // Parent process
			waitpid(_pid, &status, WUNTRACED); // Wait for the child process to finish.
		else
			perror("fork error"); //If fork fails
	}
	return (1);
}
/**
 * zimbo_path__handler - handles the path
 * Return: Found path.
 */
char *zimbo_path__handler(char **toks)
{
	char *path = NULL;
	char *path_token;
	char full_path[300];
	int i;
	//PATH= /bin:/bin/local:/usr/bin      example
	
	 // Iterate over the 'environ' array to find the 'PATH' variable
	for (i = 0; environ[i] != NULL; i++)
	{
        // Check if the current environment variable starts with "PATH="
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;  // Extract the value by skipping "PATH="
			break;
		}
	}
	if (environ[i] != NULL)
	{
		path_token = strtok(path, ":");
		while (path_token != NULL)
		{
			strcpy(full_path, path_token);  // Copy 'path' to 'full_path'
			strcat(full_path, "/");   // Append a forward slash
			strcat(full_path, toks[0]); // Append 'command'
			if (access(full_path, X_OK) == 0)
			{
				char *ammended_path = strdup(full_path);
				return (ammended_path);
			}
			path_token = strtok(NULL, ":");
		}
	}
	return (NULL);
}
