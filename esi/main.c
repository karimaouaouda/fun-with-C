#include <stdio.h>
#include "helpers.h"
#include "reader.h"
#include "traversals.h"

#define FILENAME "data.txt"



int main(){
    TreeNode* root = readFileIntoTree(FILENAME);

    printTreeInOrder(root);
    printf("\n\n");

    bottomToTopBranchTraversal(root);

    getchar();
    return 0;
}