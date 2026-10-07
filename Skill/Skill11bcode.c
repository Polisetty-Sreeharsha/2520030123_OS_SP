#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 5
#define COMMAND_SIZE 100

typedef struct
{
    char command[COMMAND_SIZE];
    int command_number;
} PipelineCommand;

typedef struct
{
    PipelineCommand commands[MAX_COMMANDS];
    int count;
} Pipeline;

void initialize_pipeline(Pipeline *pipeline)
{
    pipeline->count = 0;
}

/* Add command to pipeline */
void add_command(Pipeline *pipeline, const char *command)
{
    if (pipeline->count >= MAX_COMMANDS)
    {
        printf("Pipeline capacity exceeded.\n");
        return;
    }

    strcpy(pipeline->commands[pipeline->count].command, command);

    pipeline->commands[pipeline->count].command_number =
        pipeline->count + 1;

    pipeline->count++;
}

/* Display pipeline */
void display_pipeline(Pipeline *pipeline)
{
    int i;

    if (pipeline->count == 0)
    {
        printf("Pipeline is empty.\n");
        return;
    }

    printf("\nPipeline Structure:\n");
    printf("-------------------\n");

    for (i = 0; i < pipeline->count; i++)
    {
        printf("[%d] %s",
               pipeline->commands[i].command_number,
               pipeline->commands[i].command);

        if (i < pipeline->count - 1)
        {
            printf("  -->  ");
        }
    }

    printf("\n");
}

/* Validate pipeline */
void validate_pipeline(Pipeline *pipeline)
{
    int i;

    if (pipeline->count == 0)
    {
        printf("Pipeline validation: FAILED - empty pipeline.\n");
        return;
    }

    if (pipeline->count > MAX_COMMANDS)
    {
        printf("Pipeline validation: FAILED - too many commands.\n");
        return;
    }

    for (i = 0; i < pipeline->count; i++)
    {
        if (strlen(pipeline->commands[i].command) == 0)
        {
            printf("Pipeline validation: FAILED.\n");
            return;
        }

        if (pipeline->commands[i].command_number != i + 1)
        {
            printf("Pipeline validation: FAILED - incorrect order.\n");
            return;
        }
    }

    printf("Pipeline validation: PASSED.\n");
}

/* Display process connections */
void display_connections(Pipeline *pipeline)
{
    int i;

    printf("\nProcess Connections:\n");
    printf("--------------------\n");

    for (i = 0; i < pipeline->count; i++)
    {
        if (i == 0)
        {
            printf("%s : Standard Input\n",
                   pipeline->commands[i].command);
        }
        else
        {
            printf("%s : receives input from previous command\n",
                   pipeline->commands[i].command);
        }

        if (i < pipeline->count - 1)
        {
            printf("        |\n");
            printf("        | pipe\n");
            printf("        v\n");
        }
        else
        {
            printf("%s : Standard Output\n",
                   pipeline->commands[i].command);
        }
    }
}

int main()
{
    Pipeline pipeline;

    initialize_pipeline(&pipeline);

    printf("Pipeline Structure Manager\n");
    printf("--------------------------\n");

    /* Create pipeline */
    add_command(&pipeline, "ls");
    add_command(&pipeline, "grep .c");
    add_command(&pipeline, "wc -l");

    printf("\nCommands stored successfully.\n");

    /* Display execution order */
    display_pipeline(&pipeline);

    /* Validate pipeline */
    validate_pipeline(&pipeline);

    /* Show process connections */
    display_connections(&pipeline);

    return 0;
}
