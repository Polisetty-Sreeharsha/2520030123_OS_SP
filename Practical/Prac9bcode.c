#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int input_fd, output_fd;

    // Open input file
    input_fd = open("input.txt", O_RDONLY);

    if (input_fd == -1)
    {
        perror("Error opening input file");
        exit(1);
    }

    // Open/create output file
    output_fd = open("output.txt",
                     O_WRONLY | O_CREAT | O_TRUNC,
                     0644);

    if (output_fd == -1)
    {
        perror("Error opening output file");
        close(input_fd);
        exit(1);
    }

    /*
     * Redirect standard input.
     * File descriptor 0 = stdin
     */
    if (dup2(input_fd, STDIN_FILENO) == -1)
    {
        perror("dup2 input failed");
        exit(1);
    }

    /*
     * Redirect standard output.
     * File descriptor 1 = stdout
     */
    if (dup2(output_fd, STDOUT_FILENO) == -1)
    {
        perror("dup2 output failed");
        exit(1);
    }

    // Original descriptors are no longer needed
    close(input_fd);
    close(output_fd);

    /*
     * Now stdin comes from input.txt
     * and stdout goes to output.txt.
     */

    char buffer[100];

    printf("This line is redirected to output.txt.\n");

    if (read(STDIN_FILENO, buffer, sizeof(buffer) - 1) > 0)
    {
        buffer[sizeof(buffer) - 1] = '\0';

        printf("Data read from redirected stdin:\n");
        printf("%s\n", buffer);
    }

    return 0;
}
