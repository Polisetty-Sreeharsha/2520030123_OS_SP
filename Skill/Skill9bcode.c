#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define INPUT_SIZE 100

/* Function declarations */
void command_pwd(char *args[]);
void command_cd(char *args[]);
void command_help(char *args[]);
void command_exit(char *args[]);

/* Structure for built-in commands */
typedef struct
{
    char *name;
    void (*function)(char *args[]);
} BuiltinCommand;

/* Dispatch table */
BuiltinCommand builtins[] =
{
    {"pwd", command_pwd},
    {"cd", command_cd},
    {"help", command_help},
    {"exit", command_exit}
};

int builtin_count = sizeof(builtins) / sizeof(builtins[0]);

/* pwd command */
void command_pwd(char *args[])
{
    char current_dir[PATH_MAX];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("pwd");
        return;
    }

    printf("%s\n", current_dir);
}

/* cd command */
void command_cd(char *args[])
{
    char *path;

    if (args[1] == NULL)
    {
        path = getenv("HOME");

        if (path == NULL)
        {
            printf("cd: HOME not set\n");
            return;
        }
    }
    else
    {
        path = args[1];
    }

    if (chdir(path) == -1)
    {
        perror("cd");
    }
}

/* help command */
void command_help(char *args[])
{
    printf("\nBuilt-in commands:\n");
    printf("  pwd   - Display current working directory\n");
    printf("  cd    - Change current directory\n");
    printf("  help  - Display available commands\n");
    printf("  exit  - Exit the shell\n");
}

/* exit command */
void command_exit(char *args[])
{
    printf("Exiting shell...\n");
    exit(0);
}

/* Find command in dispatch table */
int find_builtin(char *command)
{
    int i;

    for (i = 0; i < builtin_count; i++)
    {
        if (strcmp(command, builtins[i].name) == 0)
        {
            return i;
        }
    }

    return -1;
}

int main()
{
    char input[INPUT_SIZE];
    char *args[10];
    char *token;
    int argc;
    int index;

    printf("Built-in Command Shell\n");
    printf("----------------------\n");
    printf("Type 'help' to see available commands.\n");

    while (1)
    {
        printf("\nshell> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            continue;
        }

        /* Tokenize input */
        argc = 0;
        token = strtok(input, " ");

        while (token != NULL && argc < 9)
        {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }

        args[argc] = NULL;

        /* Find command */
        index = find_builtin(args[0]);

        if (index == -1)
        {
            printf("Invalid command: %s\n", args[0]);
            printf("Type 'help' to see available commands.\n");
            continue;
        }

        /* Execute built-in command */
        builtins[index].function(args);
    }

    return 0;
}
