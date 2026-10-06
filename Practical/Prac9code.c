#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define BUFFER_SIZE 4096

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Low-level system call copy
void low_level_copy(const char *source, const char *destination)
{
    int src, dest;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    src = open(source, O_RDONLY);

    if (src == -1)
    {
        perror("Error opening source file");
        exit(1);
    }

    dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (dest == -1)
    {
        perror("Error opening destination file");
        close(src);
        exit(1);
    }

    // Demonstrating lseek()
    lseek(src, 0, SEEK_SET);

    while ((bytes_read = read(src, buffer, BUFFER_SIZE)) > 0)
    {
        bytes_written = write(dest, buffer, bytes_read);

        if (bytes_written != bytes_read)
        {
            perror("Error writing file");
            close(src);
            close(dest);
            exit(1);
        }
    }

    if (bytes_read == -1)
        perror("Error reading file");

    close(src);
    close(dest);
}

// Standard library copy
void standard_copy(const char *source, const char *destination)
{
    FILE *src, *dest;
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    src = fopen(source, "rb");

    if (src == NULL)
    {
        perror("Error opening source file");
        exit(1);
    }

    dest = fopen(destination, "wb");

    if (dest == NULL)
    {
        perror("Error opening destination file");
        fclose(src);
        exit(1);
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, src)) > 0)
    {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(src);
    fclose(dest);
}

int main()
{
    const char *source = "input.txt";
    const char *low_level_dest = "copy_lowlevel.txt";
    const char *standard_dest = "copy_standard.txt";

    double start, end;
    double low_level_time, standard_time;

    printf("File Copy Performance Comparison\n");
    printf("--------------------------------\n");

    // Low-level copy
    start = get_time();

    low_level_copy(source, low_level_dest);

    end = get_time();

    low_level_time = end - start;

    printf("Low-level system call copy completed.\n");
    printf("Time taken: %.6f seconds\n\n", low_level_time);

    // Standard library copy
    start = get_time();

    standard_copy(source, standard_dest);

    end = get_time();

    standard_time = end - start;

    printf("Standard library copy completed.\n");
    printf("Time taken: %.6f seconds\n\n", standard_time);

    printf("Performance Comparison:\n");
    printf("Low-level I/O : %.6f seconds\n", low_level_time);
    printf("Standard I/O  : %.6f seconds\n", standard_time);

    return 0;
}
