#include "helpers.h"
#include "bintree.h"
#include <stdlib.h>
#include <stdio.h>

// Optional: Function to print the binary tree (in-order traversal)
void printTreeInOrder(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    printTreeInOrder(root->left);
    printf("%d ", root->val);
    printTreeInOrder(root->right);
}