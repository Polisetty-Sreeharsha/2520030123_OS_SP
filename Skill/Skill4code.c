#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKENS 20
#define MAX_INPUT 200
typedef struct {
    char *value;
} Token;
void parse_input(char *input) {
    Token tokens[MAX_TOKENS];
    int count = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL && count < MAX_TOKENS) {
        tokens[count].value = token;
        count++;
        token = strtok(NULL, " \t\n");
    }
    printf("\n--- Token Stream ---\n");
    if (count == 0) {
        printf("No tokens found.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("Token %d: [%s]\n", i + 1, tokens[i].value);
    }
    printf("\nTotal tokens: %d\n", count);
}
int main() {
    char input[MAX_INPUT];
    printf("Enter a command: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    parse_input(input);
    return 0;
}
