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
typedef List *TListProcessus;

void creeArbre(TArbreProcessus *arbre, const char *name, int id, int priority, int ram);

void creeProcessus(TArbreProcessus *arbre, TListProcessus *prets, int pereid, char *nom, int id, int priority, int ram, int *stockage);

int lancerUnProcessus(TArbreProcessus *arbre, TListProcessus *elus);

void blockerUnProcessus(TArbreProcessus *arbre, TListProcessus *blocks, int pid);

void deblockerProcessus(TArbreProcessus *arbre, TListProcessus *blocks, int pid);

void voirArbre(TArbreProcessus arbre, TArbreProcessus pere);

void tuerProcessus(TArbreProcessus *arbre, TListProcessus *blocks, TListProcessus *elus, TListProcessus *prets, int *STOCKAGE, int id);

void extraire(TListProcessus* head, int value);

int exist(TListProcessus list, int id);