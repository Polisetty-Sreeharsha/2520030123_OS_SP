#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 4096

double get_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

/* Traditional read/write method */
void traditional_copy(const char *source, const char *destination)
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

    close(src);
    close(dest);
}

/* Memory-mapped method */
void mmap_copy(const char *source, const char *destination)
{
    int src, dest;
    struct stat file_info;
    size_t file_size;
    char *source_map;
    char *dest_map;

    src = open(source, O_RDONLY);

    if (src == -1)
    {
        perror("Error opening source file");
        exit(1);
    }

    if (fstat(src, &file_info) == -1)
    {
        perror("fstat failed");
        close(src);
        exit(1);
    }

    file_size = file_info.st_size;

    if (file_size == 0)
    {
        printf("Source file is empty.\n");
        close(src);
        return;
    }

    /* Map source file into memory */
    source_map = mmap(NULL,
                      file_size,
                      PROT_READ,
                      MAP_PRIVATE,
                      src,
                      0);

    if (source_map == MAP_FAILED)
    {
        perror("mmap source failed");
        close(src);
        exit(1);
    }

    /* Create destination file */
    dest = open(destination,
                O_RDWR | O_CREAT | O_TRUNC,
                0644);

    if (dest == -1)
    {
        perror("Error creating destination file");
        munmap(source_map, file_size);
        close(src);
        exit(1);
    }

    /* Set destination file size */
    if (ftruncate(dest, file_size) == -1)
    {
        perror("ftruncate failed");
        munmap(source_map, file_size);
        close(src);
        close(dest);
        exit(1);
    }

    /* Map destination file into memory */
    dest_map = mmap(NULL,
                    file_size,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    dest,
                    0);

    if (dest_map == MAP_FAILED)
    {
        perror("mmap destination failed");
        munmap(source_map, file_size);
        close(src);
        close(dest);
        exit(1);
    }

    /* Copy data through memory */
    memcpy(dest_map, source_map, file_size);

    /* Write changes back to file */
    if (msync(dest_map, file_size, MS_SYNC) == -1)
    {
        perror("msync failed");
    }

    munmap(source_map, file_size);
    munmap(dest_map, file_size);

    close(src);
    close(dest);
}

int main()
{
    const char *source = "mmap_input.txt";
    const char *traditional_dest = "copy_read_write.txt";
    const char *mmap_dest = "copy_mmap.txt";

    double start, end;
    double traditional_time, mmap_time;

    printf("Memory-Mapped I/O vs Traditional I/O\n");
    printf("-------------------------------------\n");

    /* Traditional read/write */
    start = get_time();

    traditional_copy(source, traditional_dest);

    end = get_time();

    traditional_time = end - start;

    printf("Traditional read/write copy completed.\n");
    printf("Time taken: %.6f seconds\n\n", traditional_time);

    /* mmap */
    start = get_time();

    mmap_copy(source, mmap_dest);

    end = get_time();

    mmap_time = end - start;

    printf("Memory-mapped copy completed.\n");
    printf("Time taken: %.6f seconds\n\n", mmap_time);

    printf("Performance Comparison:\n");
    printf("Traditional read/write : %.6f seconds\n", traditional_time);
    printf("mmap()                 : %.6f seconds\n", mmap_time);

    printf("\nFiles created successfully:\n");
    printf("- %s\n", traditional_dest);
    printf("- %s\n", mmap_dest);

    return 0;
}
