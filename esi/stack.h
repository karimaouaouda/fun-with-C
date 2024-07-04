#ifndef __STACK_
#define __STACK__

#include "bintree.h"

#define MAX_STACK_SIZE 100

struct Stack {
    struct TreeNode* nodes[MAX_STACK_SIZE];
    int top;
};
struct Stack* createStack();
int isStackEmpty(struct Stack* stack);
void push(struct Stack* stack, struct TreeNode* node);
struct TreeNode* pop(struct Stack* stack);


#endif