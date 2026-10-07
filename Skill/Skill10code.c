#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int main()
{
    char current_dir[PATH_MAX];
    char saved_dir[PATH_MAX];
    char input[100];

    /* Retrieve current directory */
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("getcwd");
        return 1;
    }

    printf("Current Directory Program\n");
    printf("-------------------------\n");
    printf("Current directory: %s\n", current_dir);

    while (1)
    {
        printf("\nshell> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nInput ended. Exiting...\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        /* Process exit request */
        if (strcmp(input, "exit") == 0)
        {
            /* Save current state */
            if (getcwd(saved_dir, sizeof(saved_dir)) == NULL)
            {
                perror("getcwd");
                break;
            }

            printf("Saving current directory: %s\n", saved_dir);
            printf("Cleaning up resources...\n");
            printf("Exiting program.\n");

            break;
        }

        /* Display current directory */
        if (strcmp(input, "pwd") == 0)
        {
            if (getcwd(current_dir, sizeof(current_dir)) == NULL)
            {
                perror("getcwd");
                continue;
            }

            printf("Current directory: %s\n", current_dir);
        }
        else
        {
            printf("Unknown command: %s\n", input);
            printf("Use 'pwd' or 'exit'.\n");
        }
    }

    return 0;
}
