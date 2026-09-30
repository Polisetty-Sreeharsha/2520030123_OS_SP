#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void builtin_pwd()
{
    char cwd[500];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}

void builtin_cd(char *path)
{
    if (path == NULL)
    {
        printf("cd: missing argument\n");
        return;
    }

    if (chdir(path) != 0)
        perror("cd");
}

void builtin_exit()
{
    printf("Exiting shell...\n");
    exit(0);
}

int main()
{
    char input[500];
    char *command;
    char *argument;

    while (1)
    {
        printf("myshell> ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        command = strtok(input, " ");
        argument = strtok(NULL, " ");

        if (command == NULL)
            continue;

        if (strcmp(command, "pwd") == 0)
        {
            builtin_pwd();
        }
        else if (strcmp(command, "cd") == 0)
        {
            builtin_cd(argument);
        }
        else if (strcmp(command, "exit") == 0)
        {
            builtin_exit();
        }
        else
        {
            printf("Invalid command: %s\n", command);
        }
    }

    return 0;
}
