#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void expandVariables(char input[], char output[])
{
    int i = 0, j = 0;

    while (input[i] != '\0')
    {
        if (input[i] == '$')
        {
            i++;

            if (input[i] == '{')
            {
                char name[100];
                int k = 0;

                i++;

                while (input[i] != '\0' && input[i] != '}')
                {
                    name[k++] = input[i++];
                }

                name[k] = '\0';

                if (input[i] == '}')
                    i++;

                char *value = getenv(name);

                if (value != NULL)
                {
                    strcpy(&output[j], value);
                    j += strlen(value);
                }
                else
                {
                    printf("Undefined variable: %s\n", name);
                }
            }
            else if (isalpha(input[i]) || input[i] == '_')
            {
                char name[100];
                int k = 0;

                while (isalnum(input[i]) || input[i] == '_')
                {
                    name[k++] = input[i++];
                }

                name[k] = '\0';

                char *value = getenv(name);

                if (value != NULL)
                {
                    strcpy(&output[j], value);
                    j += strlen(value);
                }
                else
                {
                    printf("Undefined variable: %s\n", name);
                }
            }
            else
            {
                output[j++] = '$';
            }
        }
        else
        {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
}

int main()
{
    char input[500];
    char output[1000];

    printf("Enter text: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    expandVariables(input, output);

    printf("Expanded text: %s\n", output);

    return 0;
}
