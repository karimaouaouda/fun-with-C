#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "biblioteque.h"

#define Elu  0
#define Pret  1
#define Blocker 2

int main()
{
    int choice;

    TArbreProcessus arbre = NULL;
    TListProcessus elus = NULL;
    TListProcessus blocks = NULL;
    TListProcessus prets = NULL;

    char nom[12];
    int priority;
    int ram;
    int id;
    int STOCKAGE;


    creeArbre(&arbre,  "root", 0, 1, 1);

    arbre->Etat = Elu;    
    printf("stckage : ");
    scanf("%d", &STOCKAGE);


    

    while (true)
    {
        printf("choiser :\n");
        puts("1 - cree un processus");
        puts("2 - lancer un processus");
        puts("3 - blocker un processus");
        puts("4 - deblocker un processus");
        puts("5 - tuer un processus");
        puts("6 - terminer un processus");
        puts("7 - voir les processus");

        scanf("%d", &choice);

        fflush(stdin);


        switch (choice)
        {
        case 1:

            printf("entreer un nom : ");
            gets(nom);
            printf("entreer l id de pere : ");
            scanf("%d", &id);
            printf("entreer le stockage pour reserve <%d : " , STOCKAGE );
            scanf("%d", &ram);
            printf("entreer la priority <%d : " , 7 );
            scanf("%d", &priority);
            int temp = id;
            printf("entreer un id : " , 7 );
            scanf("%d", &id);

            if(STOCKAGE < ram){
                printf("stockage insuffisant");
                break;
            }

            creeProcessus(&arbre,&prets, temp, nom, id, priority, ram, &STOCKAGE );

            puts("processus cree avec succes") ;


            break;
        case 2:

            id = lancerUnProcessus(&arbre, &elus);

            if( id != -1 ){
                printf("lancement de processus avec l'id : %d est effectue avec succes\n", id );

                extraire(&blocks, id);
                extraire(&prets, id);
            }else{
                printf(" il n'y a pas un processus pour lancer ou la file de lancemment est full\n ");
            }
            break;
        case 3:
            
            printf("entreer un id pour blocker : " );
            scanf("%d", &id);

            blockerUnProcessus(&arbre, &blocks, id);
            printf("processus est blocker");


            break;
        case 4:

            printf("entreer un id pour deblocker : " );
            scanf("%d", &id);

            deblockerProcessus(&arbre, &blocks, id);
            printf("processus est blocker");

            break;
        case 5:
            printf("entreer un id pour tuer : " );
            scanf("%d", &id);

            tuerProcessus(&arbre, &blocks, &elus, &prets, &STOCKAGE, id);
            printf("processus est tuer");
            break;
        case 6:
            printf("entreer un id pour terminer : " );
            scanf("%d", &id);

            if( exist(elus, id) == 0 ){
                printf("assurez quer le processus est elue\n");
                break;
            }

            tuerProcessus(&arbre, &blocks, &elus, &prets, &STOCKAGE, id);
            printf("processus est terminer");
            break;
        case 7:
            voirArbre(arbre, NULL);
            break;;
        case 0:
            exit(EXIT_SUCCESS);
            break;

        default:
            break;
        }
    }

    return 0;
}
