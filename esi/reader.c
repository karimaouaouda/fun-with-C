#include "reader.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "bintree.h"


TreeNode* readFileIntoTree(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    TreeNode* root = NULL;
    int number;
    while (fscanf(file, "%d", &number) != EOF) {
        root = insertNode(root, number);
    }

    fclose(file);
    return root;
}