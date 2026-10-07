#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct Node {
    char data;
    struct Node *left, *right;
} Node;

typedef struct {
    Node *items[MAX];
    int top;
} NodeStack;

typedef struct {
    int items[MAX];
    int top;
} IntStack;

Node *createNode(char data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

void pushNode(NodeStack *stack, Node *node) {
    stack->items[++stack->top] = node;
}

Node *popNode(NodeStack *stack) {
    return stack->items[stack->top--];
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

Node *buildTree(const char *postfix) {
    NodeStack stack = { .top = -1 };

    for (int i = 0; postfix[i] != '\0'; i++) {
        if (postfix[i] == ' ')
            continue;

        Node *node = createNode(postfix[i]);

        if (isOperator(postfix[i])) {
            node->right = popNode(&stack);
            node->left = popNode(&stack);
        }

        pushNode(&stack, node);
    }

    return popNode(&stack);
}

void printTree(Node *root, int space) {
    if (root == NULL)
        return;

    space += 5;
    printTree(root->right, space);

    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%c", root->data);

    printTree(root->left, space);
}

void preorder(Node *root) {
    if (root) {
        printf("%c ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(Node *root) {
    if (root) {
        if (isOperator(root->data)) printf("(");
        inorder(root->left);
        printf("%c", root->data);
        inorder(root->right);
        if (isOperator(root->data)) printf(")");
    }
}

void postorder(Node *root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        printf("%c ", root->data);
    }
}

int calculate(char op, int a, int b) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    return a / b;
}

int evaluatePostfix(const char *postfix) {
    IntStack stack = { .top = -1 };

    printf("Trace:\n");

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (c == ' ')
            continue;

        if (!isOperator(c)) {
            stack.items[++stack.top] = c - '0';
        } else {
            int b = stack.items[stack.top--];
            int a = stack.items[stack.top--];
            int result = calculate(c, a, b);

            printf("%d %c %d = %d\n", a, c, b, result);
            stack.items[++stack.top] = result;
        }
    }

    return stack.items[stack.top];
}

int evaluateTree(Node *root) {
    if (!isOperator(root->data))
        return root->data - '0';

    int left = evaluateTree(root->left);
    int right = evaluateTree(root->right);
    int result = calculate(root->data, left, right);

    printf("%d %c %d = %d\n", left, root->data, right, result);
    return result;
}

int main() {
    const char *postfix = "8 3 2 * + 6 2 / -";
    Node *root = buildTree(postfix);

    printf("Expression Tree:");
    printTree(root, 0);

    printf("\n\nPreorder: ");
    preorder(root);

    printf("\nInorder: ");
    inorder(root);

    printf("\nPostorder: ");
    postorder(root);

    printf("\n\nStack-based postfix evaluation:\n");
    printf("Result = %d\n", evaluatePostfix(postfix));

    printf("\nExpression tree evaluation:\n");
    printf("Result = %d\n", evaluateTree(root));

    printf("\nComparison:\n");
    printf("Number of arithmetic operations: 4\n");
    printf("Data structure: Stack for postfix; Binary tree for expression tree\n");
    printf("Time complexity: O(n) for both\n");
    printf("Space: O(n) for both\n");

    printf("\nThe expression tree provides structural information because it\n");
    printf("stores the relationship between every operator and its operands.\n");

    return 0;
