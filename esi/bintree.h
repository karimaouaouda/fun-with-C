#ifndef __BINTREE__
#define __BINTREE__

#include <stdbool.h>

// Define the structure for the tree node with a visited attribute
typedef struct TreeNode {
    int val;
    bool visited;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;


// Function to create a new tree node
TreeNode* createNode(int val);

TreeNode* insertNode(TreeNode* root, int data);

#endif