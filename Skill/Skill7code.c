#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

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
        printf("Child process started.\n");
        printf("Child PID: %d\n", getpid());

        sleep(2);

        printf("Child process completed.\n");
        exit(0);
    }
    else
    {
        printf("Parent is waiting for child process...\n");

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return 1;
        }

        if (WIFEXITED(status))
        {
            printf("Child process terminated normally.\n");
            printf("Child exit status: %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("Child process did not terminate normally.\n");
        }

        printf("Parent process completed.\n");
    }

    return 0;
}
