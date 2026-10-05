#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 256
#define MAX_STACK 100
#define TOKEN_SIZE 16

typedef struct Node {
    char token[TOKEN_SIZE];
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    int items[MAX_STACK];
    int top;
} IntStack;

typedef struct {
    Node *items[MAX_STACK];
    int top;
} NodeStack;

Node *createNode(const char *token) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strncpy(node->token, token, TOKEN_SIZE - 1);
    node->token[TOKEN_SIZE - 1] = '\0';
    node->left = NULL;
    node->right = NULL;
    return node;
}

void initIntStack(IntStack *stack) {
    stack->top = -1;
}

void initNodeStack(NodeStack *stack) {
    stack->top = -1;
}

void pushInt(IntStack *stack, int value) {
    if (stack->top >= MAX_STACK - 1) {
        fprintf(stderr, "Integer stack overflow.\n");
        exit(EXIT_FAILURE);
    }
    stack->items[++stack->top] = value;
}

int popInt(IntStack *stack) {
    if (stack->top < 0) {
        fprintf(stderr, "Integer stack underflow.\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top--];
}

void pushNode(NodeStack *stack, Node *node) {
    if (stack->top >= MAX_STACK - 1) {
        fprintf(stderr, "Node stack overflow.\n");
        exit(EXIT_FAILURE);
    }
    stack->items[++stack->top] = node;
}

Node *popNode(NodeStack *stack) {
    if (stack->top < 0) {
        fprintf(stderr, "Node stack underflow.\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top--];
}

int isNumber(const char *token) {
    if (*token == '\0') return 0;

    if (*token == '-' && token[1] != '\0') {
        token++;
    }

    while (*token) {
        if (!isdigit((unsigned char)*token)) return 0;
        token++;
    }
    return 1;
}

int applyOperator(const char *op, int left, int right) {
    if (strcmp(op, "+") == 0) return left + right;
    if (strcmp(op, "-") == 0) return left - right;
    if (strcmp(op, "*") == 0) return left * right;
    if (strcmp(op, "/") == 0) {
        if (right == 0) {
            fprintf(stderr, "Division by zero.\n");
            exit(EXIT_FAILURE);
        }
        return left / right;
    }

    fprintf(stderr, "Unknown operator: %s\n", op);
    exit(EXIT_FAILURE);
}

Node *buildExpressionTree(const char *postfix) {
    char expression[MAX_EXPR];
    char *token;
    NodeStack stack;

    strncpy(expression, postfix, MAX_EXPR - 1);
    expression[MAX_EXPR - 1] = '\0';
    initNodeStack(&stack);

    token = strtok(expression, " \t\n");
    while (token != NULL) {
        if (isNumber(token)) {
            pushNode(&stack, createNode(token));
        } else if (strlen(token) == 1 &&
                   strchr("+-*/", token[0]) != NULL) {
            Node *node = createNode(token);
            node->right = popNode(&stack);
            node->left = popNode(&stack);
            pushNode(&stack, node);
        } else {
            fprintf(stderr, "Invalid postfix token: %s\n", token);
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, " \t\n");
    }

    if (stack.top != 0) {
        fprintf(stderr, "Invalid postfix expression.\n");
        exit(EXIT_FAILURE);
    }

    return popNode(&stack);
}

void printTree(Node *root, int depth) {
    if (root == NULL) return;

    printTree(root->right, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("%s\n", root->token);

    printTree(root->left, depth + 1);
}

void preorder(Node *root) {
    if (root == NULL) return;
    printf("%s ", root->token);
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node *root) {
    if (root == NULL) return;

    if (root->left || root->right) printf("(");
    inorder(root->left);
    printf("%s", root->token);
    inorder(root->right);
    if (root->left || root->right) printf(")");
}

void postorder(Node *root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%s ", root->token);
}

int evaluateTree(Node *root, int trace) {
    int left, right, result;

    if (root == NULL) return 0;

    if (!root->left && !root->right) {
        return atoi(root->token);
    }

    left = evaluateTree(root->left, trace);
    right = evaluateTree(root->right, trace);
    result = applyOperator(root->token, left, right);

    if (trace) {
        printf("  %d %s %d = %d\n", left, root->token, right, result);
    }

    return result;
}

int evaluatePostfix(const char *postfix) {
    char expression[MAX_EXPR];
    char *token;
    IntStack stack;

    strncpy(expression, postfix, MAX_EXPR - 1);
    expression[MAX_EXPR - 1] = '\0';
    initIntStack(&stack);

    printf("Postfix evaluation trace:\n");

    token = strtok(expression, " \t\n");
    while (token != NULL) {
        if (isNumber(token)) {
            int value = atoi(token);
            pushInt(&stack, value);
            printf("  Read %s -> push %d\n", token, value);
        } else {
            int right = popInt(&stack);
            int left = popInt(&stack);
            int result = applyOperator(token, left, right);

            printf("  Read %s -> pop %d, pop %d -> %d %s %d = %d -> push %d\n",
                   token, right, left, left, token, right, result, result);

            pushInt(&stack, result);
        }

        token = strtok(NULL, " \t\n");
    }

    return popInt(&stack);
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printComparison(void) {
    printf("\n=== Comparison ===\n");
    printf("%-25s %-25s %-25s\n",
           "Aspect", "Stack-based postfix", "Expression tree");
    printf("%-25s %-25s %-25s\n",
           "-------------------------",
           "-------------------------",
           "-------------------------");
    printf("%-25s %-25s %-25s\n",
           "Main work",
           "Scan tokens and calculate",
           "Build tree, then traverse");
    printf("%-25s %-25s %-25s\n",
           "Data structure",
           "Operand stack",
           "Binary tree + node stack");
    printf("%-25s %-25s %-25s\n",
           "Evaluation time",
           "O(n)",
           "O(n)");
    printf("%-25s %-25s %-25s\n",
           "Extra space",
           "O(n) worst case",
           "O(n) for tree + O(h) call stack");
    printf("%-25s %-25s %-25s\n",
           "Structural information",
           "None",
           "Operator/operand hierarchy");
}

int main(void) {
    const char *postfix = "8 3 2 * + 6 2 / -";

    printf("==============================================\n");
    printf(" Expression Tree and Postfix Evaluation\n");
    printf("==============================================\n");
    printf("Postfix expression: %s\n\n", postfix);

    /* Part (a): construct and display the expression tree. */
    Node *root = buildExpressionTree(postfix);

    printf("Expression tree (sideways; right subtree appears above):\n");
    printTree(root, 0);

    printf("\nTraversals:\n");
    printf("Preorder : ");
    preorder(root);
    printf("\nInorder  : ");
    inorder(root);
    printf("\nPostorder: ");
    postorder(root);
    printf("\n");

    /* Part (b): evaluate using a stack. */
    printf("\n=== Stack-based postfix evaluation ===\n");
    int postfixResult = evaluatePostfix(postfix);
    printf("Result = %d\n", postfixResult);

    /* Part (b): evaluate the expression tree. */
    printf("\n=== Expression tree evaluation ===\n");
    printf("Tree evaluation trace:\n");
    int treeResult = evaluateTree(root, 1);
    printf("Result = %d\n", treeResult);

    if (postfixResult == treeResult) {
        printf("\nBoth methods produce the same result: %d\n", treeResult);
    } else {
        printf("\nERROR: evaluation results differ.\n");
    }

    /* Part (c): compare both approaches. */
    printComparison();

    printf("\n=== Why the tree gives more information ===\n");
    printf("The postfix stack computes the final value but discards the\n");
    printf("operator/operand relationships after each operation. The tree\n");
    printf("keeps those relationships explicitly, so it can be traversed\n");
    printf("as prefix, infix, or postfix, printed, analyzed, optimized,\n");
    printf("or evaluated again without reparsing the original expression.\n");

    freeTree(root);
    return 0;
}
