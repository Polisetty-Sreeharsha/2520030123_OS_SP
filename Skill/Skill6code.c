#include <stdio.h>
#include <string.h>

void parseInput(char input[])
{
    char output[1000];
    int i = 0, j = 0;
    int escaped = 0;
    char quote = '\0';

    while (input[i] != '\0')
    {
        char ch = input[i];

        if (escaped)
        {
            output[j++] = ch;
            escaped = 0;
        }
        else if (ch == '\'' || ch == '"')
        {
            if (quote == '\0')
                quote = ch;
            else if (quote == ch)
                quote = '\0';
            else
                output[j++] = ch;
        }
        else if (ch == '\\')
        {
            escaped = 1;
        }
        else
        {
            output[j++] = ch;
        }

        i++;
    }

    if (escaped)
    {
        printf("Error: incomplete escape sequence.\n");
        return;
    }

    if (quote != '\0')
    {
        printf("Error: unmatched quote.\n");
        return;
    }

    output[j] = '\0';

    printf("Parsed output: %s\n", output);
}

int main()
{
    char input[1000];

    printf("Enter a command or text: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    parseInput(input);

    return 0;
}
