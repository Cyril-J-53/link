# Expression Tree and Postfix Evaluation in C

This project solves the expression-tree assignment for:

```
8 3 2 * + 6 2 / -
```

## Expression

The postfix expression represents:

```
(8 + (3 * 2)) - (6 / 2)
```

So the final result is:

```
11
```

## Features

The program demonstrates all parts of the assignment:

### a) Expression Tree

- Builds an expression tree from the postfix expression using a stack of tree nodes.
- Displays the tree sideways.
- Prints:
  - Preorder
  - Inorder
  - Postorder

Expected traversals:

```
Preorder : - + 8 * 3 2 / 6 2
Inorder  : ((8+(3*2))-(6/2))
Postorder: 8 3 2 * + 6 2 / -
```

### b) Two evaluation methods

1. **Stack-based postfix evaluation**
   - Reads tokens from left to right.
   - Pushes operands.
   - Pops two operands for each operator.
   - Performs the operation and pushes the result.
   - Prints an important-operation trace.

2. **Expression tree evaluation**
   - Recursively evaluates the left and right subtrees.
   - Applies each operator after both operands are available.
   - Prints each arithmetic operation.

Both methods produce:

```
11
```

### c) Comparison

| Aspect | Stack-based postfix | Expression tree |
|---|---|---|
| Main work | Scan tokens and calculate | Build tree, then traverse |
| Data structure | Operand stack | Binary tree + node stack |
| Evaluation time | O(n) | O(n) |
| Extra space | O(n) worst case | O(n) for tree + O(h) call stack |
| Structural information | Final calculation only | Explicit operator/operand hierarchy |

For this expression there are **8 tokens**, **5 operands**, and **4 binary operators**. Both evaluation methods perform 4 arithmetic operations. The tree approach additionally performs the work needed to construct the tree.

## Why the expression tree provides more structural information

A direct postfix evaluator is optimized for getting the answer. Once an operation is completed, the temporary operands and result on the stack do not preserve the complete expression structure.

The expression tree stores every operand and operator as a node and connects them according to the expression's hierarchy. Therefore, the same tree can be used to:

- generate prefix notation,
- generate infix notation with parentheses,
- generate postfix notation,
- visualize operator precedence,
- inspect subexpressions,
- perform repeated evaluations,
- support later optimization or symbolic processing.

## Compile and run

### GCC

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c -o expression_tree
./expression_tree
```

### Windows MinGW

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c -o expression_tree.exe
expression_tree.exe
```

## Expected result

The important final lines are:

```
Result = 11
Result = 11

Both methods produce the same result: 11
```

## Assignment mapping

- **Part (a):** `buildExpressionTree()`, `printTree()`, and traversal functions.
- **Part (b):** `evaluatePostfix()` and `evaluateTree()`.
- **Part (c):** `printComparison()` and the final structural-information explanation.
