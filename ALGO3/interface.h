
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

typedef List* TList;


char** split(const char* str , char sep , int* size);

char **fetchArgs(char *argv[], int argc);

char *trim(char *str, char c);

int getNumber();

bool idExistsIn(TList liste, int pid);

TArbreProcessus nouveauProcessus(const char *name, int ram, int priority, int id);

TList pushInList(TList liste, int pid);

void pushChild(TArbreProcessus parent, TArbreProcessus fils);

TArbreProcessus processusFromArbre(TArbreProcessus root, int pid);

TArbreProcessus searchBinaire(TArbreProcessus root);

int listeSize(TList list);

TList insererEnList(TList liste, int pid);

int tuerProcessus(TArbreProcessus* root, int pid);

TList pop(TList list, int id);
TList push(TList list, int id);

void voirArbre(TArbreProcessus root);

TArbreProcessus searchBinaireId(TArbreProcessus root, int pid);
