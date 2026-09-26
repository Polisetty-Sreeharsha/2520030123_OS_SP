#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 200
#define MAX_TOKENS 20

typedef struct Node {
    char value[50];
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(const char *value) {
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;

    return node;
}

void print_tree(Node *root, int level) {
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("|-- %s\n", root->value);

    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);
}

void free_tree(Node *root) {
    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main() {
    char input[MAX_INPUT];
    char *tokens[MAX_TOKENS];
    int count = 0;

    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    /* Handle empty command */
    if (strlen(input) == 0) {
        printf("Error: Empty command.\n");
        return 1;
    }

    /* Tokenize input */
    char *token = strtok(input, " \t");

    while (token != NULL && count < MAX_TOKENS) {
        tokens[count++] = token;
        token = strtok(NULL, " \t");
    }

    /* Validate syntax */
    if (count == 0) {
        printf("Syntax Error: No command found.\n");
        return 1;
    }

    /* Detect invalid command */
    if (strcmp(tokens[0], "run") != 0 &&
        strcmp(tokens[0], "execute") != 0 &&
        strcmp(tokens[0], "print") != 0) {

        printf("Syntax Error: Unknown command '%s'.\n", tokens[0]);
        return 1;
    }

    /* Create parse tree */
    Node *root = create_node(tokens[0]);

    Node *current = root;

    for (int i = 1; i < count; i++) {
        current->left = create_node(tokens[i]);
        current = current->left;
    }

    /* Display parse tree */
    printf("\n--- Parse Tree ---\n");
    print_tree(root, 0);

    /* Produce execution structure */
    printf("\n--- Execution Structure ---\n");
    printf("Command: %s\n", root->value);

    if (count > 1) {
        printf("Arguments:\n");

        for (int i = 1; i < count; i++) {
            printf("  Argument %d: %s\n", i, tokens[i]);
        }
    } else {
        printf("No arguments.\n");
    }

    printf("\nSyntax validation successful.\n");

    free_tree(root);

    return 0;
}
