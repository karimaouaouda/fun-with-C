#include "traversals.h"
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"


void dfs(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    if (!root->visited) {
        printf("%d ", root->val);
        root->visited = true;
    }
    if (root->left && !root->left->visited) {
        dfs(root->left);
    }
    if (root->right && !root->right->visited) {
        dfs(root->right);
    }
}

void bottomToTopLevelOrderTraversal_rl(struct TreeNode* root) {
    if (root == NULL) return;

    struct Queue* queue = createQueue();
    struct Stack* stack = createStack();
    
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        int levelSize = (queue->rear - queue->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        
        // Collect all nodes of the current level
        struct TreeNode** levelNodes = (struct TreeNode**)malloc(levelSize * sizeof(struct TreeNode*));
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* node = dequeue(queue);
            levelNodes[i] = node;

            // Enqueue right child first, then left child
            if (node->right) {
                enqueue(queue, node->right);
            }
            if (node->left) {
                enqueue(queue, node->left);
            }
        }

        // Push current level nodes onto the stack
        for (int i = 0; i < levelSize; i++) {
            push(stack, levelNodes[i]);
        }
        
        free(levelNodes);
    }

    // Pop all items from the stack to get nodes in reverse level order
    while (!isStackEmpty(stack)) {
        struct TreeNode* node = pop(stack);
        printf("%d ", node->val);
    }

    // Free the memory allocated for queue and stack
    free(queue);
    free(stack);
}



void bottomToTopLevelOrderTraversal_lr(struct TreeNode* root) {
    if (root == NULL) return;

    struct Queue* queue = createQueue();
    struct Stack* stack = createStack();
    
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        int levelSize = (queue->rear - queue->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        
        // Collect all nodes of the current level
        struct TreeNode** levelNodes = (struct TreeNode**)malloc(levelSize * sizeof(struct TreeNode*));
        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* node = dequeue(queue);
            levelNodes[i] = node;

            // Enqueue left child first, then right child
            if (node->left) {
                enqueue(queue, node->left);
            }
            if (node->right) {
                enqueue(queue, node->right);
            }
        }

        // Push current level nodes onto the stack
        for (int i = 0; i < levelSize; i++) {
            push(stack, levelNodes[i]);
        }
        
        free(levelNodes);
    }

    // Pop all items from the stack to get nodes in reverse level order
    while (!isStackEmpty(stack)) {
        struct TreeNode* node = pop(stack);
        printf("%d ", node->val);
    }

    // Free the memory allocated for queue and stack
    free(queue);
    free(stack);
}

void bottomToTopBranchTraversal(struct TreeNode* root) {
    if (root == NULL) return;

    struct Queue* queue = createQueue();
    struct Stack* stack = createStack();

    push(stack, root); // Push the root onto the stack

    while (!isStackEmpty(stack)) {
        int levelSize = stack->top + 1; // Size of current level

        // Collect all nodes of the current level
        struct TreeNode** levelNodes = (struct TreeNode**)malloc(levelSize * sizeof(struct TreeNode*));
        for (int i = 0; i < levelSize; i++) {
            levelNodes[i] = pop(stack); // Pop nodes from the stack
            // Enqueue left child first, then right child
            if (levelNodes[i]->left) {
                push(stack, levelNodes[i]->left);
            }
            if (levelNodes[i]->right) {
                push(stack, levelNodes[i]->right);
            }
        }

        // Enqueue nodes of the current level in reverse order
        for (int i = levelSize - 1; i >= 0; i--) {
            enqueue(queue, levelNodes[i]);
        }

        free(levelNodes);
    }

    // Dequeue and print nodes from the queue to get bottom-to-top branch order
    while (!isQueueEmpty(queue)) {
        struct TreeNode* node = dequeue(queue);
        printf("%d ", node->val);
    }

    // Free the memory allocated for queue and stack
    free(queue);
    free(stack);
}

void bb_lr(TreeNode* root){
    if( root != NULL ){

        bbl_r(root->left);
        bb_lr(root->right);

    }
}

