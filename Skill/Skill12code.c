#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {
        /* First command: ls */
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[1]);

        execlp("ls", "ls", NULL);

        perror("execlp");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    {
        /* Second command: wc -l */
        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);

        close(pipefd[0]);

        execlp("wc", "wc", "-l", NULL);

        perror("execlp");
        exit(EXIT_FAILURE);
    }

    /* Parent closes both pipe descriptors */
    close(pipefd[0]);
    close(pipefd[1]);

    /* Wait for both child processes */
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Pipeline execution completed successfully.\n");

    return 0;
}
