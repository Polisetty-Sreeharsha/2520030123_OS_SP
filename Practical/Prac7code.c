#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 10;
static int static_var = 20;

void sample_function()
{
    printf("Code address   : %p\n", (void *)sample_function);
}

int main()
{
    int stack_var = 30;
    int *heap_var = malloc(sizeof(int));

    if (heap_var == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *heap_var = 40;

    printf("Process ID     : %d\n\n", getpid());

    sample_function();

    printf("Global address : %p\n", (void *)&global_var);
    printf("Static address : %p\n", (void *)&static_var);
    printf("Heap address   : %p\n", (void *)heap_var);
    printf("Stack address  : %p\n", (void *)&stack_var);

    printf("\nMemory mappings from /proc/%d/maps:\n\n", getpid());

    char command[100];
    snprintf(command, sizeof(command), "cat /proc/%d/maps", getpid());

    system(command);

    free(heap_var);

    return 0;
}
