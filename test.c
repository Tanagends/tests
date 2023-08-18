#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_INPUT_LENGTH 100
#define MAX_PATH_LENGTH 100
// Function to split input into command and arguments
void split_input(char *input, char *command, char *arguments[]) {
    char *token = strtok(input, " \t\n");
    strcpy(command, token);
    int i = 0;
    while (token != NULL) {
        arguments[i] = token;
        token = strtok(NULL, " \t\n");
        i++;
    }
    arguments[i] = NULL;
}
// Function to execute commands
void execute_command(char *command, char *arguments[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(command, arguments);
        perror("Command execution error");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        wait(NULL);
    } else {
        // Fork error
        perror("Fork error");
    }
}
int main() {
    char input[MAX_INPUT_LENGTH];
    char command[MAX_PATH_LENGTH];
    char *arguments[MAX_INPUT_LENGTH];
    while (1) {
        printf("simple_shell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle Ctrl+D (end of file)
            printf("\n");
            break;
        }
        // Remove the newline character
        input[strcspn(input, "\n")] = '\0';
        split_input(input, command, arguments);
        if (strcmp(command, "exit") == 0) {
            // Exit the shell
            break;
        } else if (strcmp(command, "") != 0) {
            execute_command(command, arguments);
        }
    }
    return 0;
}
