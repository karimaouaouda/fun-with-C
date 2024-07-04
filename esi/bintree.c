#include "bintree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

TreeNode* createNode(int data){
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->val = data;
    newNode->visited = false; // Initialize visited to false
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


// Function to insert a node into the binary tree
TreeNode* insertNode(TreeNode* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->val) {
        root->left = insertNode(root->left, data);
    } else if (data > root->val) {
        root->right = insertNode(root->right, data);
    }
    return root;
}