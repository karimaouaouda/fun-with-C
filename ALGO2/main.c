#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

int main(){

    char* inpt = calloc(100, sizeof(char));

    int run = true;
    
    int stockage = 0;

    

    do{
        printf("entre le stockage : ");

        gets(inpt);

    }while( atoi(inpt) > 500 || atoi(inpt) < 5 );

    stockage = atoi(inpt);

    int  id = 0;

    TArbreProcessus root = newArbreNoed("root", 1, 0, ELU, id );

    TListeProcessus EluList = insertInTListe(NULL, root->pid);

    TListeProcessus PretListe = NULL;

    TListeProcessus BlockedListe = NULL;

    TListeProcessus WaitingListe = NULL;

    while( run == true ){

        //first we need to get the chice from the user 

        puts("choiser :\n");
        puts("-(c/C) pour cree une processus (apres les donnes ...)");
        puts("-(l/L) pour lancer une processus (apres les donnes ...)");
        puts("-(b/B) pour blocker une processus (apres les donnes ...)");
        puts("-(d/D) pour deblocker une processus (apres les donnes ...)");
        puts("-(k/K) pour kill une processus (apres les donnes ...)");
        puts("-(t/T) pour terminer une processus (apres les donnes ...)");
        puts("-(h/H) pour voir l'arbre de processus (apres les donnes ...)");
        puts("-(a/A) aborter le program");

        puts(" -- example : c root newnode 14 2 --");

        gets(inpt);

        if(!isValidCommand(inpt)){
            printf("\t sil te plait entre un lettre ( c/C l/L b/B d/D k/K t/T )\n");
            continue;
        }else{

            if(inpt[0] == 'a' || inpt[0] == 'A'){
                printf("bye");
                return 0;
            }

            if( inpt[0] == 'c' || inpt[0] == 'C' ){
                printf("creation de prosususs : \n");
                printf("entre args : <parent id> <prosussus nom> <size> <priority> =>");

                gets(inpt);

                int pid;
                char* nom = malloc( 100 );
                int size;
                int priority;

                sscanf(inpt, "%d %s %d %d",&pid, nom, &size, &priority );

                if(size < 1 || priority < 0 && priority > 7 || strlen(nom) > 11 ||strlen(nom) <= 0){
                    printf("not valid inputs size or inputs or nom\n");
                }else{

                    if(size > stockage){
                        printf("insufisant stockage\n");
                        free(nom);
                        continue;
                    }

                    //check if the id is exists in run
                    if( !findInListe(EluList, pid) ){
                        printf("le processus avec id %d n'est pas en mode elu ou n'existe pas\n",pid);

                        free(nom);

                        continue;
                    }

                    if(size >= stockage){
                        printf("espace unsuffisant pour cree ccette prosussus");
                        free(nom);
                        continue;
                    }

                    id++;

                    stockage = stockage - size;

                    root = insertInArbre(root, pid,  newArbreNoed(nom, size, priority, PRET, id) );

                    PretListe = insertInTListe(PretListe, id);

                    free(nom);
                    continue;
                }



            }


            if( inpt[0] == 'l' || inpt[0] == 'L' ){
                printf("lancement de prosususs ... \n");
                
                if( listeSize(EluList) == __MAX_ELU__ ){
                    printf("cpu full\n");
                    continue;
                }

                
                
                TArbreProcessus p = findBestPriority(root);

                printf("sqd");

                if(p == NULL){
                    printf("no processus can lanch\n");
                    continue;
                }

                p->Etat = ELU;
                EluList = setElu(EluList, p->pid);

                printf("processus avec id : %d (priorite : %d) est lancer \n", p->pid, p->priority);

                continue;

            }

            if( inpt[0] == 'b' || inpt[0] == 'B' ){
                printf("blocking de prosususs : \n");
                printf("entre args : <pocessuss id> =>");

                gets(inpt);

                int pid;

                sscanf(inpt, "%d", &pid);

                if(!findInListe(EluList, pid)){
                    printf("can not block an not elu processus\n");
                    continue;
                }

                EluList = removeFromListe(EluList, pid);

                BlockedListe = insertInTListe(BlockedListe, pid);

                updateEtat(root, pid, BLOCK);

                continue;
            }

            if( inpt[0] == 'd' || inpt[0] == 'D' ){
                printf("deblocking de prosususs : \n");
                printf("entre args : <pocessuss id> =>");

                gets(inpt);

                int pid;

                sscanf(inpt, "%d", &pid);

                if(!findInListe(BlockedListe, pid)){
                    printf("can not block an not blocked processus\n");
                    continue;
                }

                BlockedListe = removeFromListe(BlockedListe, pid);

                PretListe = insertInTListe(PretListe, pid);
                

                updateEtat(root, pid, PRET);
            }

            if( inpt[0] == 'k' || inpt[0] == 'K' ){
                printf("kill de prosususs : \n");
                printf("entre args : <pocessuss id> =>");

                gets(inpt);

                int pid;

                sscanf(inpt, "%d", &pid);

                stockage = stockage + taille(root, pid);

                removeFromArbre(root, pid, &EluList, &BlockedListe, &PretListe);
            }

            if( inpt[0] == 't' || inpt[0] == 'T' ){
                printf("terminer de prosususs : \n");
                printf("entre args : <pocessuss id> =>");

                gets(inpt);

                int pid;

                sscanf(inpt, "%d", &pid);

                if( !findInListe(EluList,pid) ){
                    printf("can not terminer an not ELU processus\n");
                    continue;
                }

                stockage = stockage + taille(root, pid);

                root = removeFromArbre(root, pid, &EluList, &BlockedListe, &PretListe);
            }
        
            if( inpt[0] == 'h' || inpt[0] == 'H' ){
                voirArbre(root);
                continue;
            }
        
        }

    }
}