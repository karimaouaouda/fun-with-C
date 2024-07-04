#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define __MAX_ELU__ 4



#define ELU 0
#define BLOCK 1
#define PRET 2

typedef struct Processus
{
    int pid;

    char nom[12];

    int Etat;

    int RAM;

    int priority;

    struct Processus *Fils;
    struct Processus *Frere;

} Processus;

typedef Processus *TArbreProcessus;

typedef struct List
{
    int pid;
    struct List *suivant;
} List;

typedef List *TListeProcessus;

TArbreProcessus newArbreNoed(const char *name, int ram, int priority, int statut, int id);

bool isValidCommand(const char *inpt);

bool isValidCommand(const char *inpt);

TListeProcessus makeListeNode(int pid);

TListeProcessus insertInTListe(TListeProcessus liste, int pid);

TArbreProcessus findInArbre(TArbreProcessus root, int pid);

int listeSize(TListeProcessus list);

TListeProcessus setElu(TListeProcessus liste, int pid);

TArbreProcessus insertArbreChild(TArbreProcessus parent, TArbreProcessus child);

TArbreProcessus insertInArbre(TArbreProcessus root, int pid, TArbreProcessus child);

bool findInListe(TListeProcessus liste, int pid);

bool updateEtat(TArbreProcessus root, int pid, int newEtat);

TArbreProcessus findBestPriority(TArbreProcessus root);

TListeProcessus removeFromListe(TListeProcessus list, int pid);

TArbreProcessus removeArbre(TArbreProcessus root, TListeProcessus *elus, TListeProcessus *blocked, TListeProcessus *pret);

TArbreProcessus removeFromArbre(TArbreProcessus root, int pid, TListeProcessus *elus, TListeProcessus *blocked, TListeProcessus *pret);

void voirArbre(TArbreProcessus root);

int taille(TArbreProcessus root, int pid);
