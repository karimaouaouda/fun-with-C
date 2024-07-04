#ifndef __USED_ARBRE_STRUCTURE
#define __USED_ARBRE_STRUCTURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_types.h"

Arbre* createArbreNode(Student* student){
    Arbre* node = (Arbre*) malloc(sizeof(Arbre));

    node->data = student;

    node->left = node->right = NULL;

    return node;
}

Arbre* insertIntoArbre(Arbre* root, Student* student){
    if(root == NULL){
        Arbre* node = createArbreNode(student);

        return node;
    }

    if( student->matricule <  root->data->matricule ){
        root->left = insertIntoArbre(root->left, student);
    }else{
        root->right = insertIntoArbre(root->right, student);
    }


    return root;
}

Arbre* extractData(const char* filename, int* size, double* time){

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE* file = fopen(filename, "r");

    if(!file){
        fprintf(stderr, "failed to open stream :  no such file named %s", filename);
        exit(EXIT_FAILURE);
    }

    int count = 0; //count of the students to read
    char* line =  (char*) malloc(sizeof(char) * 200); //we assume that at most the line has 200 character
    Arbre* arbre = NULL;
    Student* student;

    while (!feof(file))
    { // lire vers la fin de fichier
        if (fgets(line, sizeof(line), file) != NULL)
        {
            //we will read the n student so we to increament the count to 
            //make it equal to n
            count ++;

            student = (Student*) malloc(sizeof(Student));

            //read the line into the new student (the new student automatically it's index is (n - 1) or (count - 1 in our case))
            sscanf(line, "Matricule: %d nom: %s prenom: %s groupe: %d", &(student->matricule), student->nom, student->prenom, &(student->group) );
        
            arbre = insertIntoArbre(arbre, student);
        
        }
    }


    *size = count;

    end = clock(); // Record the end time

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the time difference

    *time = cpu_time_used; //example
    fclose(file);

    return arbre;
}

double sortData(Data* data, int algorithm){
    puts("the binary tree sort the data in inserting time\n we use search binary tree to perform that\nisn't that awesome :)");

    return 0.0;
}

#endif