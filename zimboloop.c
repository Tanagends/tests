#include "zimbo.h"
/**
 * main - a simple shell
 * Return: 0(Always success).
 */
char *builtstr[] = {"cd", "setenv", "env", "exit", "unsetenv"};
int (*builtfunc[])(char **toks) = {zimbo_cd, zimbo_setenv, zimbo_env,
	zimbo_exit, zimbo_unset_env};
// command prompt duplicating upon null input
// double free error
// error messages
// executing a command only on the first prompt
extern char** environ;
int main(void)
{
	char *input, **toks;
	size_t size = 0;
	ssize_t i, k;
	int status = 1;

	while (status)
	{
		write(1, "Zimboshell$ ", 12);
		//if (i != -1)
	//	{
			k = getline(&input, &size, stdin);
		//	if (k != -1)
		//	{
				if (feof(stdin))
				{
					write(1, "\n", 1);
					break;
				}
				toks = zimbo_split(input);
			//else
			//	printf("Error\n");
			//	perror(toks[0]);
				status = zimbo_execute(toks);
			//	free(input);
				free(toks);
		//	}
			//else if (input == "\n")
			//	write(1, "\n", 1);
		//}
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
	int builtins = -1;
	char *path_handler_backup;

	if (toks[0] == NULL)
		return (1);
	builtins = zimbo_builtins(toks);
	if (builtins != -1)
		return (builtins);
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
				//strcpy(path_handler_backup, path_handler);
				//free(path_handler);
				if (execve(path_handler, toks, environ) == -1)
				{
					perror(toks[0]);
				//	free(path_handler);
					exit(errno);
				}
				//else
					//free(path_handler);
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
	else
		perror("Error");
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
/**
 * zimbo_builtins - checks and execute builtins.
 * @toks: tokenized string.
 */
int zimbo_builtins(char **toks)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		if (strcmp(builtstr[i], toks[0]) == 0)
			return ((*builtfunc[i])(toks));
	}
	return (-1);
}
/**
 * exit - exits the shell.
 * Return: 0 (Success)
 */
int zimbo_exit(char **toks)
{
	if (toks[0] != NULL)
		return (0);
}
/**
 * zimbo_cd - changes directory.
 * @toks: tokenized input.
 * Return: 1 (Success)
 */
int zimbo_cd(char **toks)
{
	int i, k = 0;
	char owd[MAX_LINE], pwd[MAX_LINE], nwd[MAX_LINE];

	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror("getcwd error");
		return (1);
	}
	if (toks[1] == NULL)
		i = chdir("~");
	else if (toks[2] != NULL)
	{
		printf("Too many arguments");
		return (1);
	}
	else if (toks[1] == "-")
	{
		i = handle_cd(toks);
	}
	else
		i = chdir(toks[1]);
	if (i == 0)
	{
		if (getcwd(nwd, sizeof(nwd)) != NULL)
			set_old_new_pwd(pwd, nwd);
	}
	else
		perror(toks[1]);
	return (1);
}
/**
 * set_old_new_pwd - updates PWD and OLDPWD.
 * @cwd: OLDPWD value.
 * @nwd: PWD value.
 */
void set_old_new_pwd(char *pwd, char *nwd)
{
	char old[MAX_LINE], new[MAX_LINE], *oldp, *newp;

	//memset(old, 0, sizeof(old));
	memset(new, 0, sizeof(new));
	//strcpy(old, "setenv OLDPWD ");
	//strcat(old, pwd);
	//strcat(old, "1");
	strcpy(new, "setenv PWD ");
        strcat(new, nwd);
        strcat(new, "1");
	printf("%s\n", new);
	//oldp = malloc(strlen(old) + 1);
	//newp = malloc(strlen(new) + 1);
	//if (newp == NULL)
	//{
	//	perror("Malloc error");
	//	return;
	//}
	//zimbo_setenv(zimbo_split(oldp));
	zimbo_setenv(zimbo_split(new));
	//free(oldp);
	//free(newp);
}
/**
 * handle_cd- - handles cd-.
 * @toks: tokenized input.
 * Return: 1 (Success)
 */
int handle_cd(char **toks)
{
	char *old_wd = NULL, cwd[MAX_LINE];
	int k = 0, i;

	while (environ[k] != NULL)
	{
		if (strncmp(environ[k], "OLDPWD=", 7) == 0)
		{
			old_wd = environ[k] + 7;
			if (getcwd(cwd, sizeof(cwd)) == NULL)
				return (1);
			i = chdir(old_wd);
			break;
		}
	}
	if (environ[k] == NULL)
	{
		perror("Old directory not found");
		return (1);
	}
	return (i);
}
/**
 * zimbo_env - prints environment variables.
 * @toks: tokenized input.
 * Return: 1 (Success).
 */
int zimbo_env(char **toks)
{
	int i = 0;

	if (toks[1] != NULL)
		return (1);
	while (environ[i] != NULL)
	{
		write(1, environ[i], strlen(environ[i]));
		write(1, "\n", 1);
		i++;
	}
	return (1);
}
/**
 * zimbo_setenv - sets environment variables.
 * @toks: tokenized input.
 * Return: Always 1 (Success).
 */
int zimbo_setenv(char **toks)
{
	int i = 0;
	char new_env[MAX_LINE], *new_envp;

	if (toks[4] != NULL)
	{
		printf("Too many arguments");
		return (1);
	}
	while (environ[i] != NULL)
	{
		if (strncmp(environ[i++], toks[1], strlen(toks[1])) == 0)
		{
			if (toks[3] == 0)
				return (1);
			else
			{
				zimbo_unset_env(toks);
				break;
			}
		}
	}
	strcpy(new_env, toks[1]);
	strcat(new_env, "=");
	strcat(new_env, toks[2]);
	new_envp = malloc(strlen(new_env) + 1);
	if (new_envp == NULL)
	{
		perror("malloc");
		return (1);
	}
	strcpy(new_envp, new_env);
	environ[i] = new_envp;
	environ[i + 1] = NULL;
	return (1);
}
/**
 * zimbo_unset_env - unsets an environment variable.
 * @toks: tokenized input.
 * Return: integer.
 */
//We might need to alter the return values to match the actual return values of the actual functions.
int zimbo_unset_env(char **toks)
{
	int i = 0, k = 0;

	if (toks[1] == NULL)
	{
		perror("Command not found");
		return (1);
	}
	while (environ[i] != NULL)
	{
		if (strncmp(environ[i], toks[1], strlen(toks[1])) == 0)
			break;
		i++;
	}
	if (environ[i] == NULL)
	{
		perror("Variable not found");
		return (1);
	}
	while (environ[k] != NULL)
		k++;
	k--;
	environ[i] = environ[k];
	environ[k] = NULL;
	return (1);
}
