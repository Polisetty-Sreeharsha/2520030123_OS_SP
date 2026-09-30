#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    char *args[] = {"ls", "-l", NULL};

    printf("Parent process started.\n");
    printf("Parent PID: %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild process created.\n");
        printf("Child PID: %d\n", getpid());

        printf("Executing ls -l...\n\n");

        execvp(args[0], args);

        perror("execvp failed");
        exit(1);
    }
    else
    {
        printf("\nParent is waiting for child...\n");

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return 1;
        }

        if (WIFEXITED(status))
        {
            printf("\nChild process finished.\n");
            printf("Exit status: %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("\nChild process did not exit normally.\n");
        }
    }

    return 0;
}
