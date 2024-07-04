#ifndef __TRAVERSALS__
#define __TRAVERSALS__
#include "bintree.h"

// Example traversal algorithm using the visited attribute (Depth-First Search)
void dfs(TreeNode* root);

void levelOrderTraversal(struct TreeNode* root);

void bottomToTopLevelOrderTraversal_rl(struct TreeNode* root);

void bottomToTopLevelOrderTraversal_lr(struct TreeNode* root);

void bottomToTopBranchTraversal(struct TreeNode* root);

#endif