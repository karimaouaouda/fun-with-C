#ifndef _CUSTOM_TYPES
#define _CUSTOM_TYPES

#include <stdlib.h>
#include <stdio.h>


char _DATA_FILENAME_[100];
#define _RAPPORT_FILENAME_ "rapport.txt"
#define _SORTED_FILE_ "sorted_list.txt"

#define _DATA_FOLDER_ "./data"



#define __BUBBLE_SORT_ALGO 1
#define __INSERTION_SORT_ALGO 3
#define __SELECTION_SORT_ALGO 2
#define __QUICK_SORT_ALGO 4

#define _CHOSE_FILE_ 0
#define _EXTRACT_METHODS_ 1
#define _SORT_ALGO_CHOSE_ 2
#define _INCLUDE_HASH_ 3
#define __PIPELINE__SIZE__ 2 // first chose extrct method, then sort algorithm, the hash table

#define _EXTRACT_AS_ARRAY_ 1
#define _EXTRACT_AS_LINKEDLIST_ 2
#define _EXTRACT_AS_ARBRE_ 3



typedef struct{
    char nom[40];
    char prenom[40];
    int matricule;
    int group;
}Student;

typedef struct Node{
    Student* data;
    struct Node* next;
}List;


typedef struct Arbre{
    Student* data;
    struct Arbre* left;
    struct Arbre* right;
}Arbre;

typedef struct Data{
    void* students;
    int structure;
    int size;
}Data;


Student* newStudent(char* nom, char* prenom, int matricule, int grp){
    Student* student = (Student*) malloc(sizeof(Student));


    strcpy(student->nom, nom);
    strcpy(student->prenom, prenom);

    student->matricule = matricule;

    student->group = grp;

    return student;
}


#endif