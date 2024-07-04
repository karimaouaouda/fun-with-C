#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interface.h"

#define ELU 0
#define BLOCK 1
#define PRET 2


int main()
{

    char command[200];

    char** parts = NULL;

    char* trimmed = NULL;

    int count;

    char* action;

    int RAM;

    TList elus = NULL;
    TList prets = NULL;
    TList blocked = NULL;

    do{

        printf("initialiser la ram (min : %d , max : %d) >> ", 0, 1000);

        gets(command);


    }while( atoi(command) < 1 || atoi(command) > 1000 );

    RAM = atoi(command);

    int parentId;

    int id = -1;

    TArbreProcessus root = nouveauProcessus("root", 1, 1, ++id);
    root->Etat = 0;

    elus = insererEnList(elus, 0);
    

    while(true){
        printf("\n----------------- ram = %d --------------------\n", RAM);
        puts("disponible commands : ");
        puts("cree <nom> <parent_id> <ram> <priority> ( exp : cree oussama 0 15 2 )");
        puts("lancer ( exp : lancer )");
        puts("tuer <process_id> ( exp : tuer 1 )");
        puts("terminer <process_id> ( exp : terminer 1 )");
        puts("voir ( exp : voir )");
        puts("exit ( exp : exit )");
        puts("----------------------------------------------------------------");

        printf("\n>>");

        gets(command);

        trimmed = trim(command, ' '); //remove space from begin and end of the string

        if(strlen(trimmed) == 0 || strlen(trimmed) < 4){

            puts("taper un command valid");

            continue;
        }

        parts = split(trimmed, ' ', &count);


        action = parts[0]; // action is the first word

        if( strcmp(action, "cree") == 0 ){
            //cree de processus
            
            //actually we need cree + root id + process name + process size + process priority
            //so we need 5 parts

            if(count < 5){

                printf("-- too few argumants to command cree --\n");
                continue;

            }

            if( strlen(parts[1]) < 1 || strlen(parts[1]) > 10 ){

                printf("-- le nom de processus must be greeat the 1 and less the 11 --\n");
                continue;

            }

            parentId = atoi(parts[2]);

            if( idExistsIn(elus, parentId) == false ){
                printf("-- le processus parent n'est pas elu --\n");
                continue;
            }
            

            if( atoi(parts[3]) < 0 || atoi(parts[3]) > RAM ){
                printf("-- le stockage reserver est great the the available or not valid --\n");
                continue;
            }

            if( atoi(parts[4]) < 1 || atoi(parts[4]) > 7 ){
                printf("-- la priorite d'un processus should be between 1 et 7 --\n");
                continue;
            }


            TArbreProcessus p = processusFromArbre(root, parentId);

            TArbreProcessus newP = nouveauProcessus(parts[1], atoi(parts[3]), atoi(parts[4]), ++id);

            pushChild(p, newP);

            prets = insererEnList(prets, newP->pid);

            RAM -= newP->RAM;

            printf("processus created successfully ;) ");
        }

        if( strcmp(action, "lancer") == 0 ){
             TArbreProcessus p = searchBinaire(root);

             if(p == NULL){
                printf("no processus can be launch");
                continue;
             }

             if(p->Etat == 0){
                printf("process est deja launched");
                continue;
             }

             if(listeSize(elus) == 4){
                printf("maximum processus launched");
                continue;
             }

             insererEnList(elus, p->pid);

             p->Etat = 0;

             printf("lancer processus : %s", p->nom);

             continue;
        }

        if( strcmp(action, "tuer") == 0 ){

            if(count < 2){

                printf("-- too few argumants to command tuer --\n");
                continue;

            }

            if( atoi(parts[1]) <= 0 ){

                printf("-- id must be number ositive >1 (pour le root juste cancel le programme) --\n");
                continue;

            }

            TArbreProcessus p = searchBinaireId(root, atoi(parts[1]));

            if(p == NULL){
                printf("sorry there is no processus with that id");
                continue;
            }

            RAM += tuerProcessus(&root, atoi(parts[1]));

            printf("le processus est tuer");

            continue;
        }

        if( strcmp(action, "blocker") == 0 ){

            if(count < 2){
                printf("too few arguments to command blocker 1 required 0 given\n");
                continue;
            }

            if( atoi(parts[1]) == 0 ){
                printf("invalid process id\n");
                continue;
            }
            TArbreProcessus p = searchBinaireId(root, atoi(parts[1]));

            if( p->Etat == BLOCK ){
                printf("le processus %s est deja blocker\n", p->nom);
                continue;
            }

            if(p->Etat == ELU){
                elus = pop(elus, p->pid);
            }

            blocked = insererEnList(blocked, p->pid);
            p->Etat = BLOCK;

            printf("le processus %s est blocker\n", p->nom);
            continue;

        }

        if( strcmp(action, "deblocker") == 0 ){
            if(count < 2){
                printf("too few arguments to command deblocker 1 required 0 given\n");
                continue;
            }

            if( atoi(parts[1]) == 0 ){
                printf("invalid process id\n");
                continue;
            }
            TArbreProcessus p = searchBinaireId(root, atoi(parts[1]));

            if( p->Etat != BLOCK ){
                printf("le processus %s must be blocker pour la deblocker\n", p->nom);
                continue;
            }

            blocked = pop(blocked, p->pid);
            prets = insererEnList(prets, p->pid);
            p->Etat = PRET;

            printf("le processus %s est deblocker\n", p->nom);
        }

        if( strcmp(action, "terminer") == 0 ){
            
            TList temp = elus;

            while (temp != NULL)
            {
                if(temp->pid == atoi(parts[1])){
                    break;
                }
                temp = temp->suivant;
            }

            if(temp == NULL){
                printf("can not terminer a processus not elu");
            }else{
                elus = pop(elus, atoi(parts[1]));

                RAM += tuerProcessus(&root, atoi(parts[1]));

                printf("processus terminer :)");
            }
            

        }

        if( strcmp(action, "voir") == 0 ){
            voirArbre(root);
        }

        if( strcmp(action, "exit") == 0 ){
            exit(EXIT_SUCCESS);
        }

    }


    return 0;
}
