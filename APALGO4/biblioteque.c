
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "biblioteque.h"



void creeArbre(TArbreProcessus *arbre, const char *name, int id, int priority, int ram)
{
    *arbre = malloc(sizeof(Processus));

    (*arbre)->Fils = (*arbre)->Frere = NULL;
    (*arbre)->pid = id;
    (*arbre)->priority = priority;
    (*arbre)->RAM = ram;
    (*arbre)->Etat = 1;

    strcpy((*arbre)->nom, name);
}

void inserer(TListProcessus *list, int id)
{
    if ((*list) == NULL)
    {
        *list = malloc(sizeof(List));
        (*list)->pid = id;

        *(&((*list)->suivant)) = NULL;
    }
    else
    {
        inserer(&((*list)->suivant), id);
    }
}

void creeProcessus(TArbreProcessus *arbre, TListProcessus *prets, int pereid, char *nom, int id, int priority, int ram, int *stockage)
{
    if ((*arbre) != NULL)
    {
        if ((*arbre)->pid == pereid)
        {
            if ((*arbre)->Etat != 0)
            {
                printf("assurez que le pere est elu\n");
                return;
            }
            TArbreProcessus *temp = &((*arbre)->Fils);

            if (temp == NULL)
            {
                creeArbre(&((*arbre)->Fils), nom, id, priority, ram);

                *stockage -= ram;

                inserer(prets, id);
            }
            else
            {
                while ((*temp) != NULL)
                {
                    temp = &((*temp)->Frere);
                }

                creeArbre(temp, nom, id, priority, ram);
                *stockage -= ram;

                inserer(prets, id);
            }
        }
        else
        {
            creeProcessus(&((*arbre)->Fils), prets, pereid, nom, id, priority, ram, stockage);
            creeProcessus(&((*arbre)->Frere), prets, pereid, nom, id, priority, ram, stockage);
        }
    }
}

int exist(TListProcessus list, int id)
{
    if (list == NULL)
    {
        return 0;
    }
    else
    {
        if (list->pid == id)
        {
            return 1;
        }
        return exist(list->suivant, id) + 0;
    }
}

void extraire(TListProcessus* head, int value) {
    TListProcessus current = *head;
    TListProcessus prev = NULL;

    // Traverse the list to find the node with the given value
    while (current != NULL && current->pid != value) {
        prev = current;
        current = current->suivant;
    }

    // If the node with the given value is found
    if (current != NULL) {
        // Update the pointers to remove the node
        if (prev != NULL) {
            prev->suivant = current->suivant;
        } else {
            *head = current->suivant;
        }

        // Free the memory occupied by the removed node
        free(current);
    }
}

void eluer(TArbreProcessus *arbre, int pid)
{
    if ((*arbre) != NULL)
    {
        if ((*arbre)->pid == pid)
        {
            (*arbre)->Etat = 0;
        }
        else
        {
            eluer(&((*arbre)->Fils), pid);
            eluer(&((*arbre)->Frere), pid);
        }
    }
}

void blocker(TArbreProcessus *arbre, int pid)
{
    if ((*arbre) != NULL)
    {
        if ((*arbre)->pid == pid)
        {
            (*arbre)->Etat = 2;
        }
        else
        {
            eluer(&((*arbre)->Fils), pid);
            eluer(&((*arbre)->Frere), pid);
        }
    }
}

void pret(TArbreProcessus *arbre, int pid)
{
    if ((*arbre) != NULL)
    {
        if ((*arbre)->pid == pid)
        {
            (*arbre)->Etat = 1;
        }
        else
        {
            pret(&((*arbre)->Fils), pid);
            pret(&((*arbre)->Frere), pid);
        }
    }
}

TArbreProcessus findMinPriorityNode(TArbreProcessus root)
{
    if (root == NULL)
    {
        return NULL;
    }

    TArbreProcessus minNode = NULL;

    // Check if the node is not running before considering it
    if (!(root->Etat == 0))
    {
        minNode = root;
    }

    TArbreProcessus leftMin = findMinPriorityNode(root->Fils);
    TArbreProcessus rightMin = findMinPriorityNode(root->Frere);

    if(minNode == NULL){
        if(leftMin == NULL || rightMin == NULL){
            return leftMin == NULL ? rightMin : leftMin;
        }else{
            return leftMin->priority < rightMin->priority ? leftMin : rightMin ;
        }
    }else{
        if(leftMin == NULL || rightMin == NULL){
            minNode = leftMin == NULL ? rightMin : leftMin;

            if(minNode == NULL) return root;

            return root->priority < minNode->priority ? root : minNode;
        }

        if( leftMin->priority < rightMin->priority ){
            minNode = leftMin;
        }else{
            minNode = rightMin;
        }

        return root->priority < minNode->priority ? root : minNode ;
    }
}

int lancerUnProcessus(TArbreProcessus *arbre, TListProcessus *elus)
{
    TListProcessus *temp = elus;

    int s = 0;
    while ((*temp) != NULL)
    {
        s = s + 1;
        (*temp) = (*temp)->suivant;
    }
    if (s == 4)
    {
        printf("cpu insufissant\n");
        return -1;
    }

    TArbreProcessus p = findMinPriorityNode(*arbre);

    if (p == NULL)
    {
        printf("n'est ya pas un process pour lancer\n");
        return -1;
    }

    int pid = p->pid;

    inserer(elus, pid);
    eluer(arbre, pid);

    return pid;
}

void blockerUnProcessus(TArbreProcessus *arbre, TListProcessus *blocks, int pid)
{
    TListProcessus *temp = blocks;
    if (exist((*blocks), pid) == 0)
    {
        inserer(blocks, pid);
        blocker(arbre, pid);
    }
    else
    {
        printf("exist in blocks\n");
    }
}

void deblockerProcessus(TArbreProcessus *arbre, TListProcessus *blocks, int pid)
{
    TListProcessus *temp = blocks;
    if (exist((*blocks), pid) == 0)
    {
        printf("le processus n'est pas blocke");
    }
    else
    {
        extraire(blocks, pid);
        pret(arbre, pid);
    }
}

void voirArbre(TArbreProcessus arbre, TArbreProcessus pere)
{
    if (arbre != NULL)
    {
        printf("noeud : nom %s (fils de %s) priority %d ram %d id %d etat %d (0 - elu 1-pret 2-blocke)\n", arbre->nom, pere == NULL ? "root" : pere->nom, arbre->priority, arbre->RAM, arbre->pid, arbre->Etat);
        voirArbre(arbre->Fils, arbre);
        voirArbre(arbre->Frere, pere);
    }
}

void suprimer(TArbreProcessus *arbre, int pid, int *ram, TListProcessus *prets, TListProcessus *elus, TListProcessus *blocks)
{
    if ((*arbre) != NULL)
    {

        if ((*arbre)->pid == pid)
        {
            suprimer(&((*arbre)->Fils), pid, ram, prets, elus, blocks);
            *ram = (*ram) + (*arbre)->RAM;
            extraire(prets, pid);
            extraire(blocks, pid);
            extraire(elus, pid);
            TArbreProcessus temp = (*arbre)->Frere;
            free(*arbre);
            *arbre = temp;
        }
        else
        {
            suprimer(&((*arbre)->Fils), pid, ram, prets, elus, blocks);
            suprimer(&((*arbre)->Frere), pid, ram, prets, elus, blocks);
            *ram = (*ram) + (*arbre)->RAM;
            extraire(prets, pid);
            extraire(blocks, pid);
            extraire(elus, pid);

            free(*arbre);
            *arbre = NULL;
        }
    }
}

void tuerProcessus(TArbreProcessus *arbre, TListProcessus *blocks, TListProcessus *elus, TListProcessus *prets, int *STOCKAGE, int id)
{
    if ((*arbre) != NULL)
    {
        if ((*arbre)->pid == id)
        {
            suprimer(arbre, id, STOCKAGE, prets, elus, blocks);
        }
        else
        {
            tuerProcessus(&((*arbre)->Fils), blocks, elus, prets, STOCKAGE, id);
            tuerProcessus(&((*arbre)->Frere), blocks, elus, prets, STOCKAGE, id);
        }
    }
}