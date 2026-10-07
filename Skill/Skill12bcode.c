#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_COMMANDS 4
#define NUM_PIPES (NUM_COMMANDS - 1)

int main()
{
    int pipes[NUM_PIPES][2];
    pid_t pids[NUM_COMMANDS];

    int i;

    /* Create all pipes */
    for (i = 0; i < NUM_PIPES; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    /* Create all processes */
    for (i = 0; i < NUM_COMMANDS; i++)
    {
        pids[i] = fork();

        if (pids[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0)
        {
            /* First command gets input normally */
            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            /* Last command sends output normally */
            if (i < NUM_COMMANDS - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            /* Close all pipe descriptors in child */
            int j;
            for (j = 0; j < NUM_PIPES; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            /* Execute appropriate command */
            if (i == 0)
            {
                execlp("ls", "ls", NULL);
            }
            else if (i == 1)
            {
                execlp("grep", "grep", ".c", NULL);
            }
            else if (i == 2)
            {
                execlp("sort", "sort", NULL);
            }
            else if (i == 3)
            {
                execlp("wc", "wc", "-l", NULL);
            }

            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    /* Parent closes all pipe descriptors */
    for (i = 0; i < NUM_PIPES; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    /* Wait for all processes */
    for (i = 0; i < NUM_COMMANDS; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    printf("Multiple-pipe pipeline completed successfully.\n");

    return 0;
}

