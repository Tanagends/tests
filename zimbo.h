#ifndef ZIMBO_H
#define ZIMBO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#define MAX_LINE 2048
#define ZIMBO_DELIM " \t\r\n\a"

char *zimbo_path__handler(char **toks);
char **zimbo_split(char *string);
int zimbo_execute(char **toks);
int zimbo_cd(char **toks);
int zimbo_env(char **toks);
int zimbo_setenv(char **toks);
int zimbo_exit(char **toks);
int zimbo_builtins(char **toks);
int zimbo_unset_env(char **toks);
void set_old_new_pwd(char *pwd, char *nwd);
int handle_cd(char **toks);
int cd_home(void);

#endif
