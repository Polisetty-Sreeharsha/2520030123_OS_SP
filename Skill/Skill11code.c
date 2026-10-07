#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5
#define COMMAND_SIZE 100

char history[HISTORY_SIZE][COMMAND_SIZE];

int history_count = 0;
int history_start = 0;

/* Add command to history */
void add_command(const char *command)
{
    int position;

    if (history_count < HISTORY_SIZE)
    {
        position = (history_start + history_count) % HISTORY_SIZE;

        strcpy(history[position], command);

        history_count++;
    }
    else
    {
        /* Buffer is full - overwrite oldest command */
        position = history_start;

        strcpy(history[position], command);

        history_start = (history_start + 1) % HISTORY_SIZE;
    }
}

/* Display history */
void display_history()
{
    int i;
    int position;

    if (history_count == 0)
    {
        printf("History is empty.\n");
        return;
    }

    printf("\nCommand History:\n");
    printf("----------------\n");

    for (i = 0; i < history_count; i++)
    {
        position = (history_start + i) % HISTORY_SIZE;

        printf("%d  %s\n", i + 1, history[position]);
    }
}

/* Retrieve a particular history entry */
void retrieve_command(int number)
{
    int position;

    if (number < 1 || number > history_count)
    {
        printf("Invalid history number.\n");
        return;
    }

    position = (history_start + number - 1) % HISTORY_SIZE;

    printf("History entry %d: %s\n",
           number,
           history[position]);
}

/* Validate history consistency */
void validate_history()
{
    int i;
    int position;

    if (history_count < 0 || history_count > HISTORY_SIZE)
    {
        printf("History consistency check: FAILED\n");
        return;
    }

    for (i = 0; i < history_count; i++)
    {
        position = (history_start + i) % HISTORY_SIZE;

        if (strlen(history[position]) == 0)
        {
            printf("History consistency check: FAILED\n");
            return;
        }
    }

    printf("History consistency check: PASSED\n");
}

int main()
{
    char command[COMMAND_SIZE];
    int number;

    printf("Command History Manager\n");
    printf("-----------------------\n");

    printf("History capacity: %d commands\n", HISTORY_SIZE);
    printf("Type 'history' to display history.\n");
    printf("Type 'get N' to retrieve an entry.\n");
    printf("Type 'check' to validate history.\n");
    printf("Type 'exit' to quit.\n");

    while (1)
    {
        printf("\nshell> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
        {
            continue;
        }

        if (strcmp(command, "exit") == 0)
        {
            printf("Exiting history manager.\n");
            break;
        }

        if (strcmp(command, "history") == 0)
        {
            display_history();
            continue;
        }

        if (strcmp(command, "check") == 0)
        {
            validate_history();
            continue;
        }

        if (sscanf(command, "get %d", &number) == 1)
        {
            retrieve_command(number);
            continue;
        }

        /* Store normal command */
        add_command(command);

        printf("Command stored: %s\n", command);
    }

    return 0;
}
