#ifndef ZIMBO_H
#define ZIMBO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/type.h>
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

#endif
