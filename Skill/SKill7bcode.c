#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    char *path;
    char *path_copy;
    char *dir;
    char full_path[1000];
    char command[100];

    printf("Enter command: ");
    scanf("%99s", command);

    path = getenv("PATH");

    if (path == NULL)
    {
        printf("PATH variable not found.\n");
        return 1;
    }

    path_copy = strdup(path);

    if (path_copy == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    dir = strtok(path_copy, ":");

    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        if (access(full_path, X_OK) == 0)
        {
            printf("Executable found: %s\n", full_path);
            free(path_copy);
            return 0;
        }

        dir = strtok(NULL, ":");
    }

    printf("Command not found: %s\n", command);

    free(path_copy);

    return 1;
}
