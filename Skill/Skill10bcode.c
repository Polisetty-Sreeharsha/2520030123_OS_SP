#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#define INPUT_SIZE 200

int valid_name(const char *name)
{
    int i;

    if (name == NULL || name[0] == '\0')
        return 0;

    if (!isalpha((unsigned char)name[0]) && name[0] != '_')
        return 0;

    for (i = 1; name[i] != '\0'; i++)
    {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_')
            return 0;
    }

    return 1;
}

void handle_export(char *command)
{
    char *equal_sign;
    char *name;
    char *value;

    /* Find '=' */
    equal_sign = strchr(command, '=');

    if (equal_sign == NULL)
    {
        printf("Invalid export syntax.\n");
        printf("Use: export NAME=value\n");
        return;
    }

    /* Separate name and value */
    *equal_sign = '\0';

    name = command + 7;
    value = equal_sign + 1;

    /* Remove spaces from variable name */
    while (*name == ' ')
        name++;

    if (!valid_name(name))
    {
        printf("Invalid variable name: %s\n", name);
        return;
    }

    /* Set environment variable */
    if (setenv(name, value, 1) == -1)
    {
        perror("setenv");
        return;
    }

    printf("Exported: %s=%s\n", name, value);
}

void show_variable(char *name)
{
    char *value;

    value = getenv(name);

    if (value == NULL)
    {
        printf("%s is not set.\n", name);
    }
    else
    {
        printf("%s=%s\n", name, value);
    }
}

void test_child_process(char *name)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        char *value = getenv(name);

        printf("Child process received: %s=%s\n",
               name,
               value != NULL ? value : "NULL");

        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

int main()
{
    char input[INPUT_SIZE];

    printf("Environment Variable Export Program\n");
    printf("------------------------------------\n");

    printf("Commands:\n");
    printf("  export NAME=value\n");
    printf("  printenv NAME\n");
    printf("  test NAME\n");
    printf("  exit\n");

    while (1)
    {
        printf("\nshell> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        /* Exit */
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting program.\n");
            break;
        }

        /* Export */
        if (strncmp(input, "export ", 7) == 0)
        {
            handle_export(input);
        }

        /* Print environment variable */
        else if (strncmp(input, "printenv ", 9) == 0)
        {
            char *name = input + 9;

            if (valid_name(name))
            {
                show_variable(name);
            }
            else
            {
                printf("Invalid variable name.\n");
            }
        }

        /* Test child process */
        else if (strncmp(input, "test ", 5) == 0)
        {
            char *name = input + 5;

            if (valid_name(name))
            {
                test_child_process(name);
            }
            else
            {
                printf("Invalid variable name.\n");
            }
        }

        else
        {
            printf("Invalid command.\n");
        }
    }

    return 0;
}
