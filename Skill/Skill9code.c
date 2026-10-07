#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

int main()
{
    char current_dir[PATH_MAX];
    char previous_dir[PATH_MAX] = "";
    char input[PATH_MAX];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("getcwd");
        return 1;
    }

    printf("Directory Navigation Program\n");
    printf("-----------------------------\n");
    printf("Current directory: %s\n", current_dir);

    while (1)
    {
        printf("\nEnter directory path");
        printf(" (use .., ., ~, or exit): ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting program.\n");
            break;
        }

        if (strcmp(input, "") == 0)
        {
            printf("Path cannot be empty.\n");
            continue;
        }

        /* Save current directory before changing */
        if (getcwd(previous_dir, sizeof(previous_dir)) == NULL)
        {
            perror("getcwd");
            continue;
        }

        /* Handle home directory */
        if (strcmp(input, "~") == 0)
        {
            char *home = getenv("HOME");

            if (home == NULL)
            {
                printf("HOME environment variable not found.\n");
                continue;
            }

            strcpy(input, home);
        }

        /* Change directory */
        if (chdir(input) == -1)
        {
            printf("Error: Cannot change to '%s': %s\n",
                   input, strerror(errno));
            continue;
        }

        /* Get updated working directory */
        if (getcwd(current_dir, sizeof(current_dir)) == NULL)
        {
            perror("getcwd");
            return 1;
        }

        printf("Directory changed successfully.\n");
        printf("Current directory : %s\n", current_dir);
        printf("Previous directory: %s\n", previous_dir);
    }

    return 0;
}
