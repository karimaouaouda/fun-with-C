#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

TArbreProcessus newArbreNoed(const char *name, int ram, int priority, int statut, int id)
{

    TArbreProcessus Processus = (TArbreProcessus)malloc(sizeof(struct Processus));

    strcpy(Processus->nom, name);

    Processus->Fils = Processus->Frere = NULL;

    Processus->pid = id;

    Processus->RAM = ram;

    Processus->priority = priority;

    Processus->Etat = statut;

    return Processus;
}

bool charIn(char c, const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (c == str[i])
        {
            return true;
        }
        i++;
    }

    return false;
}

bool isValidCommand(const char *inpt)
{
    if (strlen(inpt) <= 0 || strlen(inpt) > 1)
    {
        return false;
    }

    if (!charIn(inpt[0], "cCdDbBlLkKTteEhHa"))
    {
        return false;
    }

    return true;
}

TListeProcessus makeListeNode(int pid)
{
    TListeProcessus liste = malloc(sizeof(List));
    liste->pid = pid;
    liste->suivant = NULL;

    return liste;
}

TListeProcessus insertInTListe(TListeProcessus liste, int pid)
{
    if (liste == NULL)
    {
        return makeListeNode(pid);
    }

    liste->suivant = insertInTListe(liste->suivant, pid);
}

TArbreProcessus findInArbre(TArbreProcessus root, int pid)
{
    if (root != NULL)
    {
        if (root->pid == pid)
            return root;

        TArbreProcessus left = findInArbre(root->Fils, pid);

        if (left != NULL)
            return left;

        TArbreProcessus right = findInArbre(root->Frere, pid);

        if (right != NULL)
            return right;

        return NULL;
    }
    return NULL;
}

int listeSize(TListeProcessus list)
{
    if (list != NULL)
    {
        return 1 + listeSize(list->suivant);
    }

    return 0;
}

TListeProcessus setElu(TListeProcessus liste, int pid)
{

    if (liste != NULL || listeSize(liste) < __MAX_ELU__)
    {
        return insertInTListe(liste, pid);
    }

    return NULL;
}

TArbreProcessus insertArbreChild(TArbreProcessus parent, TArbreProcessus child)
{

    if (parent->Fils == NULL)
    {
        parent->Fils = child;
        return child;
    }

    TArbreProcessus c = parent->Fils;

    while (c->Frere != NULL)
    {
        c = c->Frere;
    }

    c->Frere = child;

    return child;
}

TArbreProcessus insertInArbre(TArbreProcessus root, int pid, TArbreProcessus child)
{
    TArbreProcessus parent = findInArbre(root, pid);

    if (parent == NULL)
    {
        return NULL;
    }

    insertArbreChild(parent, child);

    return root;
}

bool findInListe(TListeProcessus liste, int pid)
{
    if (liste == NULL)
        return false;

    if (liste->pid == pid)
        return true;

    return false || findInListe(liste->suivant, pid);
}

bool updateEtat(TArbreProcessus root, int pid, int newEtat)
{
    TArbreProcessus p = findInArbre(root, pid);

    if (p == NULL)
    {
        return false;
    }

    p->Etat = newEtat;
    return true;
}

TArbreProcessus findBestPriority(TArbreProcessus root)
{
    if (root != NULL)
    {
        TArbreProcessus left = findBestPriority(root->Fils);

        TArbreProcessus right = findBestPriority(root->Frere);

        if (left == NULL && right == NULL)
        {
            if (root->Etat == ELU)
            {
                return NULL;
            }
            else
            {
                return root;
            }
        }

        if (root->Etat == ELU)
        {
            if (left == NULL)
            {
                return right;
            }

            if (right == NULL)
            {
                return left;
            }

            return left->priority < right->priority ? left : right;
        }

        if (left == NULL)
        {
            return right->priority < root->priority ? right : root;
        }

        if (right == NULL)
        {
            return left->priority < root->priority ? left : root;
        }

        left = left->priority < root->priority ? left : root;

        return left->priority < right->priority ? left : right;
    }

    return NULL;
}

TListeProcessus removeFromListe(TListeProcessus list, int pid)
{
    if (list != NULL)
    {

        if (list->pid == pid)
        {
            return list->suivant;
        }

        list->suivant = removeFromListe(list->suivant, pid);

        return list;
    }

    return NULL;
}

TArbreProcessus removeArbre(TArbreProcessus root, TListeProcessus *elus, TListeProcessus *blocked, TListeProcessus *pret)
{
    if (root != NULL)
    {
        root->Fils = removeArbre(root->Fils, elus, blocked, pret);
        root->Frere = removeArbre(root->Frere, elus, blocked, pret);

        if (root->Etat == ELU)
        {
            *elus = removeFromListe(*elus, root->pid);
        }
        if (root->Etat == BLOCK)
        {
            *blocked = removeFromListe(*blocked, root->pid);
        }
        if (root->Etat == PRET)
        {
            *pret = removeFromListe(*pret, root->pid);
        }

        free(root);
        return NULL;
    }
    return NULL;
}

TArbreProcessus removeFromArbre(TArbreProcessus root, int pid, TListeProcessus *elus, TListeProcessus *blocked, TListeProcessus *pret)
{
    if (root != NULL)
    {
        if (root->Fils != NULL && root->Fils->pid == pid)
        {
            TArbreProcessus t = root->Fils->Frere;

            removeArbre(root->Fils->Fils, elus, blocked, pret);

            if (root->Fils->Etat == ELU)
            {
                *elus = removeFromListe(*elus, root->Fils->pid);
            }
            if (root->Fils->Etat == BLOCK)
            {
                *blocked = removeFromListe(*blocked, root->Fils->pid);
            }
            if (root->Fils->Etat == PRET)
            {
                *pret = removeFromListe(*pret, root->Fils->pid);
            }

            free(root->Fils);

            root->Fils = t;
            return root;
        }

        if (root->Frere != NULL && root->Frere->pid == pid)
        {
            TArbreProcessus t = root->Frere->Frere;

            removeArbre(root->Frere->Fils, elus, blocked, pret);

            if (root->Frere->Etat == ELU)
            {
                *elus = removeFromListe(*elus, root->Frere->pid);
            }
            if (root->Frere->Etat == BLOCK)
            {
                *blocked = removeFromListe(*blocked, root->Frere->pid);
            }
            if (root->Frere->Etat == PRET)
            {
                *pret = removeFromListe(*pret, root->Frere->pid);
            }

            free(root->Frere);

            root->Frere = t;
            return root;
        }

        root->Fils = removeFromArbre(root->Fils, pid, elus, blocked, pret);
        root->Frere = removeFromArbre(root->Frere, pid, elus, blocked, pret);

        return root;
    }

    return NULL;
}

void voirArbre(TArbreProcessus root)
{
    if (root == NULL)
    {
        return;
    }

    static int offset = 1;
    for (int i = 0; i < offset; i++)
    {
        printf("-");
    }
    printf("processus --pid=%d --name=%s --size=%d --priority=%d --Etat=%d\n", root->pid, root->nom, root->RAM, root->priority, root->Etat);
    offset += 5;
    voirArbre(root->Fils);
    offset -= 5;
    voirArbre(root->Frere);
}

int calcTaille(TArbreProcessus p){
    if( p != NULL ){

        return p->RAM + calcTaille(p->Fils) + calcTaille(p->Frere);

    }

    return 0;
}

int taille(TArbreProcessus root, int pid) {
    TArbreProcessus p = findInArbre(root, pid);

    return calcTaille(p);
}