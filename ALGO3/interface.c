#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interface.h"

// definition of fetchArgs
char **fetchArgs(char *argv[], int argc)
{
    // variable that copy the arguments without the name of the programme into it
    char **args;
    args = (char **)malloc(sizeof(char *) * (argc - 1));
    int len;
    // loop throuth the arguments
    for (int i = 0; i < argc; i++)
    {
        // get the length of the current argument
        len = strlen(argv[i]);
        // allocate a space to to be ready to copy
        args[i] = (char *)malloc(len + 1);

        // copy the argument from one to another
        memcpy(args[i], argv[i + 1], len + 1);
    }

    // return the new arguments
    return args;
}

/*
* split function that split a string in items refer to seperator
* @param char* str a string to split it
* @param char a character define the seperator
* @param int* a integer pointer that the function will store the nuber of items

* @return char** an array of strings defines the items
*/
char **split(const char *str, char sep, int *size)
{
    if (str == NULL)
    {
        return NULL;
    }

    if (strlen(str) == 0)
    {
        return NULL;
    }

    char *copy = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(copy, str);

    char *valid = trim(copy, sep);

    if (strlen(valid) == 0)
    {
        return NULL;
    }

    char **parts = malloc(sizeof(char *));
    int curr_string = 0, last_index = 0;
    parts[curr_string] = malloc(sizeof(char));
    int i = 0;

    while (true)
    {
        parts[curr_string] = realloc(parts[curr_string], ++last_index + 1);

        if (valid[i] == '\0')
        {
            parts[curr_string][last_index - 1] = '\0';
            break;
        }

        if (valid[i] == sep)
        {
            if (last_index == 1)
            {
                i++;
                last_index--;
                continue;
            }
            parts[curr_string][last_index - 1] = '\0';
            parts = realloc(parts, (++curr_string + 1) * sizeof(char *));
            parts[curr_string] = malloc(sizeof(char));
            last_index = 0;
        }
        else
        {
            parts[curr_string][last_index - 1] = valid[i];
        }
        i++;
    }

    *size = curr_string + 1;
    free(valid);
    return parts;
}

/*
* trim function that remove character from the bigining and the end of a given string
* @param char* str a string to remove from
* @param char a character define the character to remove

* @return char* a new string define the given string after remove the character from bigining and end
*/
char *trim(char *str, char c)
{
    int i = 0;             // define index of first letter which not the character to remove
    int len = strlen(str); // define the string length
    int j = len - 1;       // define the last character that does not equal to character to remove

    int last_index = 0;                          // define the last index of the new string ( to add null character )
    char *buffer = (char *)malloc(sizeof(char)); // a buffer to store the new string in it

    // if there is no string given just return a null string
    if (len <= 0)
    {
        free(str);
        buffer[last_index] = '\0';
        return buffer;
    }

    while (str[i] != '\0' && str[i] == c)
    { // moe the i forward
        i++;
    }

    while (str[j] == c)
    { // move j backward
        j--;
    }

    if (i == len - 1)
    { // if all cgaracters not the character to remove return the string itself
        buffer[last_index] = '\0';
        return buffer;
    }

    for (int f = i; f <= j; f++)
    { // coppy the left substring into the buffer
        buffer = (char *)realloc(buffer, ++last_index + 1);
        buffer[last_index - 1] = str[f];
    }
    i = 0;
    buffer[last_index] = '\0'; // set the null character

    return buffer; // return the new string
}

int getNumber()
{
    int x;

    char *str = malloc(sizeof(char) * 10);

    gets(str);

    x = atoi(str);

    if (!x)
    { // not x before atoi(char*) returning 0 if it fail
        free(str);
        return 0;
    }

    return x;
}

bool idExistsIn(TList liste, int pid)
{
    if (liste == NULL)
        return false;

    if (liste->pid == pid)
        return true;

    return false || idExistsIn(liste->suivant, pid);
}

TArbreProcessus nouveauProcessus(const char *name, int ram, int priority, int id)
{

    TArbreProcessus Processus = (TArbreProcessus)malloc(sizeof(struct Processus));

    strcpy(Processus->nom, name);

    Processus->Fils = Processus->Frere = NULL;

    Processus->pid = id;

    Processus->RAM = ram;

    Processus->priority = priority;

    Processus->Etat = 2;

    return Processus;
}

TList makeListeNode(int pid){
    TList l = malloc(sizeof(List));
    l->pid = pid;
    l->suivant = NULL;

    return l;
}


void pushChild(TArbreProcessus parent, TArbreProcessus fils)
{

    if (parent->Fils == NULL)
    {
        parent->Fils = fils;
    }
    else
    {
        TArbreProcessus temp = parent->Fils;

        while (temp->Frere != NULL)
        {
            temp = temp->Frere;
        }

        temp->Frere = fils;
    }
}

TArbreProcessus processusFromArbre(TArbreProcessus root, int pid)
{
    if (root != NULL)
    {
        if (root->pid == pid)
            return root;

        TArbreProcessus fils = processusFromArbre(root->Fils, pid);

        if (fils != NULL)
            return fils;

        TArbreProcessus frere = processusFromArbre(root->Frere, pid);

        if (frere != NULL)
            return frere;

        return NULL;
    }
    return NULL;
}

TArbreProcessus bestIn2(TArbreProcessus node1, TArbreProcessus node2){
    if(node1 == NULL){
        if(node2 == NULL || node2->Etat == 0){
            return NULL;
        }

        return node2;
    }

    if(node2 == NULL){
        if(node1 == NULL || node1->Etat == 0){
            return NULL;
        }

        return node1;
    }
    
    if(node1->Etat == 0){
        if(node2->Etat == 0){
            return NULL;
        }

        return node2;
    }

    if(node2->Etat == 0){
        if(node1->Etat == 0){
            return NULL;
        }

        return node1;
    }
    

    if(node1->priority < node2->priority){
        return node1;
    }else{
        return node2;
    }
}

TArbreProcessus bestIn3(TArbreProcessus root, TArbreProcessus fils, TArbreProcessus frere){
    if(root->Etat == 0){
        return bestIn2(fils, frere);
    }

    TArbreProcessus best = bestIn2(root, fils);

    return bestIn2(best, frere);
}

TArbreProcessus searchBinaire(TArbreProcessus root){
    if(root == NULL){
        return NULL;
    }

    TArbreProcessus fils =  searchBinaire(root->Fils);
    TArbreProcessus frere =  searchBinaire(root->Frere);

    return bestIn3(root, fils, frere);
}

int listeSize(TList list)
{
    if (list != NULL)
    {
        return 1 + listeSize(list->suivant);
    }

    return 0;
}

TList insererEnList(TList liste, int pid)
{
    if (liste == NULL)
    {
        TList l = malloc( sizeof(List) );
        l->pid = pid;
        l->suivant = NULL;

        return l;
    }

    liste->suivant = insererEnList(liste->suivant, pid);
}

void deleteArbre(TArbreProcessus* root){
    if( (*root) != NULL ){
        deleteArbre( &( (*root)->Fils ) );
        deleteArbre( &( (*root)->Frere ) );

        (*root)->Fils = (*root)->Frere = NULL;

        printf("\ntuer de processus : %s\n", (*root)->nom);

        free(*root);

        *root = NULL;
    }
}


int calcTaille(TArbreProcessus p){
    if( p != NULL ){

        return p->RAM + calcTaille(p->Fils) + calcTaille(p->Frere);

    }

    return 0;
}

int taille(TArbreProcessus root, int pid) {
    TArbreProcessus p = searchBinaireId(root, pid);

    return calcTaille(p);
}


int tuerProcessus(TArbreProcessus* root, int pid){
    if( (*root) != NULL ){

        int t = 0;
    
        if( (*root)->pid == pid ){
            t = calcTaille(*root);

            TArbreProcessus *p = &((*root)->Frere);
            TArbreProcessus *p2 = &( (*root)->Fils );
            deleteArbre(p2);

            free(*root);

            *root = *p;

            return t;

        }else{
            t = t + tuerProcessus( &( (*root)->Fils ), pid );
            t = t + tuerProcessus( &( (*root)->Frere ), pid );
            return t;
        }
    }
    return 0;
}

TList pop(TList list, int id){
    if(list != NULL){

        if(list->pid == id){
            return list->suivant;
        }

        list->suivant = pop(list->suivant, id);

        return list;

    }

    return NULL;
}

TArbreProcessus searchBinaireId(TArbreProcessus root, int pid){
    if(root != NULL){
        if(root->pid == pid){
            return root;
        }
        TArbreProcessus fils = searchBinaireId(root->Fils, pid);

        if(fils == NULL){
            return searchBinaireId(root->Frere, pid);
        }else{
            return fils;
        }
    }
    return NULL;
}

char etatsT[][6] = {
    "elu",
    "block",
    "pret"
};

void voirArbre(TArbreProcessus root)
{
    static int offset = 1;
    if (root == NULL)
    {
        return;
    }

    
    for (int i = 1; i < offset; i++)
    {
        printf("+");
    }
    printf("processus (pid=%d name=%s ram=%d priority=%d Etat=%s)\n", root->pid, root->nom, root->RAM, root->priority, etatsT[root->Etat]);
    offset += 5;
    voirArbre(root->Fils);
    offset -= 5;
    voirArbre(root->Frere);
}
