#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "bintree.h"


struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    return stack;
}

int isStackEmpty(struct Stack* stack) {
    return stack->top == -1;
}

void push(struct Stack* stack, struct TreeNode* node) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Stack is full!\n");
        return;
    }
    stack->nodes[++stack->top] = node;
}

struct TreeNode* pop(struct Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Stack is empty!\n");
        return NULL;
    }
    return stack->nodes[stack->top--];
}
