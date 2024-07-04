#include "headerfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define HIGHEST_PRIORITY 8

enum STATUS{
    Blocke,
    Pret,
    Elu
};

struct  Node
{
    unsigned int PID;
    char nom[12]; //NOM
    unsigned int ram; //RAM
    enum STATUS Etat; //etat
    unsigned int priority;
    struct Node* Fils; //fils

    struct Node* Frere; //frere
};

struct ListNode{
    ProcessesTree process;
    struct ListNode* next; //suivant
};

struct CPUStruct
{
    ProcessesList processes;

    int currentRunning;
};

//global variablles
unsigned int RAM;

//helper functions

void printChar(char c, int times){
    for(int i = 0; i < times; i++){
        printf("%c", c);
    }
}

int getNumber(bool include_zero){
    char* buffer = (char*) malloc( sizeof(char) * 100 );
    int number = 0;
    do{
        printf("=>");
        gets(buffer);

        if(strcmp(buffer, "0") == 0 && include_zero){
            return 0;
        }
    }while( (number = atoi(buffer)) <= 0 );

    free(buffer);

    return number;
}

void displayChoices(){
    printf("\nchose from this list (by number) : \n");
    
    puts("\n\t 1 - create a process");
    puts("\n\t 2 - launch a process");
    puts("\n\t 3 - terminate a process");
    puts("\n\t 4 - block a process");
    puts("\n\t 5 - deblock a process");
    puts("\n\t 6 - kill a process");
    puts("\n\t 7 - display the processes are running");
    puts("\n\t 0 - cancel the program");
}

unsigned int getChoice(unsigned int count){
    int choice = -1;
    do{
        printf("\n\t your choice ==> ");

        choice = getNumber(true);

    }while(choice > count);

    return choice;
}

bool isUnique(ProcessesTree root, const char* nom){

    if(root == NULL){
        return true;
    }

    if( strcmp(root->nom, nom) == 0 ){
        return false;
    }

    return true && isUnique(root->Fils, nom) && isUnique(root->Frere, nom);
}

ProcessesTree newProcess(const char* nom, unsigned int ram, unsigned int priority, enum STATUS statut){
    
    ProcessesTree process = (ProcessesTree) malloc(sizeof(struct Node));

    strcpy(process->nom, nom);

    process->Fils = process->Frere = NULL;

    process->PID = rand();

    process->ram = ram;

    process->priority = priority;

    process->Etat = statut;

    RAM -= ram;

    printf("=>%d <=", sizeof(process));
    return process;
}

void insertDirectFils(ProcessesTree parent, ProcessesTree Fils){
    if(parent->Fils == NULL){
        printf("inserting");
        parent->Fils = Fils;
    }else{
        ProcessesTree p = parent->Fils;

        while (p->Frere != NULL)
        {
            p = p->Frere;
        }

        p->Frere = Fils;
    }
}


ProcessesTree findProcessBynom(ProcessesTree root, const char* nom){
    if(root == NULL){
        return NULL;
    }

    if(strcmp( root->nom, nom ) == 0){
        return root;
    }

    ProcessesTree p = findProcessBynom(root->Fils, nom);

    if(p == NULL){
        return findProcessBynom(root->Frere, nom);
    }

    return p;

}


void startShowingProcesses(ProcessesTree root, unsigned int time){
    if(root != NULL){
        printf("\t\t");
        printChar('*', time );
        printf("%s ( pid=%d , ram=%d , statut=%d )\n" , root->nom, root->PID, root->ram, root->Etat);
        startShowingProcesses(root->Fils, time + 5);
        startShowingProcesses(root->Frere, time );
    }
}


//end of helper functions

CPU initCPU(unsigned int count){
    CPU cpu = (CPU) malloc( sizeof(struct CPUStruct) );

    cpu->processes = NULL;

    cpu->currentRunning = 0; //no  process is running
}

ProcessesList removeFromList(ProcessesList list, ProcessesTree process){
    if(list != NULL){

        if(process == list->process){
            return list->next;
        }

        list->next = removeFromList(list->next, process);

        return list;


    }

    return list;

}

CPU freeCPU(CPU cpu, ProcessesTree process){
    cpu->processes = removeFromList(cpu->processes, process);

    cpu->currentRunning --;
}

ProcessesTree getProcess(unsigned int pid, ProcessesTree root){
    if(root != NULL){
        //if the root or current node isn't null
        if(root->PID == pid)
            return root;


        //return the process if it is in left side or in the right side (Fils or sibling)
        ProcessesTree p = getProcess(pid, root->Fils);

        if(p == NULL){
            return getProcess(pid, root->Frere);
        }
        return p;
    }
    //return null if not
    return NULL;
}

void freeNode(ProcessesTree process, CPU cpu){
     if(process->Etat == Elu){
            freeCPU(cpu, process);
        }
        RAM += process->ram;
        process->Fils = NULL;
        process->Frere = NULL;
        free(process);
}


void freeTree(ProcessesTree root, CPU cpu){
    if(root != NULL){
        
        freeTree(root->Fils, cpu);
        freeTree(root->Frere, cpu);
        printf("\n\t!! -- info : process (%s, %d) is killed -- !!\n", root->nom, root->PID);
        freeNode(root, cpu);
    }
}

bool terminate(unsigned int pid){

}


ProcessesTree parentTreeNode(){
    ProcessesTree root = newProcess("root", 1, 1, Elu);
    root->PID = 0;
    return root;
}

unsigned int initRam(){

    unsigned int x = 0;

    do{
        printf("- set up the ram size => ");
    
        x = getNumber(false);

    }while(x < 10 || x > 500);

    return x;

}

bool strcchar(const char* str, char c){
    unsigned short i = 0;

    while(str[i] != '\0'){
        if(str[i] == c){
            return true;
        }
        i++;
    }

    return false;
}

ProcessesList newListNode(ProcessesTree process){
    ProcessesList listNode = malloc(sizeof(struct ListNode));

    listNode->process = process;
    listNode->next = NULL;

    return listNode;
}

ProcessesList insertProcessInList(ProcessesList list, ProcessesTree process){
    if(list == NULL){
        return newListNode(process);
    }
    return insertProcessInList(list->next, process);
}

bool canLaunch(ProcessesTree process, CPU cpu){
   return cpu->currentRunning < 4; 
}

void startLaunchingProcess(ProcessesTree process, CPU cpu){
    cpu->currentRunning ++;

    cpu->processes = insertProcessInList(cpu->processes, process);

    process->Etat = Elu;
}

void startBlockingProcess(ProcessesTree process, CPU cpu){
    process->Etat = Blocke;
    cpu->processes = removeFromList(cpu->processes, process);
}

void startKillingProcess(ProcessesTree root, ProcessesTree process, CPU cpu){
    if(root != NULL){
        if(root->Fils != NULL){
            if(root->Fils == process){
                if(process->Etat == Elu){
                    freeCPU(cpu, process);
                }
                freeTree(root->Fils->Fils, cpu);
                ProcessesTree t = root->Fils->Frere;
                freeNode(root->Fils, cpu);
                root->Fils = t;
                return;
            }else{
                startKillingProcess(root->Fils, process, cpu);
            }
        }

        if(root->Frere != NULL){
            if(root->Frere == process){
                if(process->Etat == Elu){
                    freeCPU(cpu, process);
                }
                freeTree(root->Frere->Fils, cpu);
                ProcessesTree t = root->Frere->Frere;
                freeNode(root->Frere, cpu);
                root->Frere = t;
                return;
            }else{
                startKillingProcess(root->Frere, process, cpu);
            }
        }
    }
}

ProcessesTree getBestFrom(ProcessesTree first, ProcessesTree second){
    if(first == NULL){
        return second;
    }

    if(second == NULL){
        return first;
    }

    if(first->Etat != Elu && second->Etat != Elu ){
        return first->priority < second->priority ? first : second;
    }

    return first->Etat == Elu ? second : first;
}

ProcessesTree getLowestPriority(ProcessesTree root, unsigned int lowest){
    if(root != NULL){
        ProcessesTree bestFromFils = getLowestPriority(root->Fils, root->priority);
        ProcessesTree bestFromSibling = getLowestPriority(root->Frere, root->priority);

        ProcessesTree best =  getBestFrom(bestFromFils, bestFromSibling);

        return getBestFrom(best, root);

    }

    return NULL;
}

void startDeblockingProcess(ProcessesTree process){
    process->Etat = Pret;
}
//here the main functions

/**
 * @brief 
*/
void createProcess(ProcessesTree root, CPU cpu){
    char* inputBuffer = (char*) malloc( sizeof(char) * 50 );
    printf("\n\t-the nom of the parent process => ");

    gets(inputBuffer);

    if(strlen(inputBuffer) > 11 || strlen(inputBuffer) <= 0 ){
        printf("\n !! -- error : the nom must be between 1-11 character length -- !!  \n");
        return;
    }
    
    ProcessesTree parent = findProcessBynom(root, inputBuffer);

    if(parent == NULL){
        printf("\n\t !! -- error : no process with that nom -- !!  \n");
        return;
    }

    printf("\n\t -- info : parent process founded with PID =>  %d --\n", parent->PID);

    if(parent->Etat != Elu){
        printf("\n\t !! -- error : trying to create a process from unactive process -- !!  \n");
        return;
    }

    

    char* nom = malloc(sizeof(char) * 12);
    printf("\n\t-the process nom (length < 11) => ");
    gets(inputBuffer);

    if(strlen(inputBuffer) > 11 || strlen(inputBuffer) <= 0){
        printf("\n\t !! -- error : nom must be less the 11 char -- !!  \n");
        return;
    }

    if(!isUnique(root, inputBuffer)){
        printf("\n\t !! -- error : nom must be lunique, %s aready taken -- !!  \n", inputBuffer);
        return;
    }

    strcpy(nom, inputBuffer);

    

    printf("\n\t-ram size of the process ( 1< ram <%d ) => ", RAM);
    int ram = getNumber(false);

    if(ram > RAM){
        printf("\n\t !! -- error : max size exeeded, trying to allocate %d in %d of avaiable ram -- !!  \n", ram, RAM);
        return;
    }

    printf("\n\t-the priority of the process ( 1< ram <%d ) => ", RAM);
    int priority = getNumber(false);

    if(priority > 7){
        printf("\n\t !! -- error : priorities are between 1-7 -- !!  \n", ram, RAM);
        return;
    }

    printf("\n\t !! --   process createing...   -- !!\n");
    ProcessesTree process = newProcess(nom, ram, priority, Pret );

     printf("\n\t !! --   inserting to the processe (%d) tree...   -- !!\n", process->PID);

    insertDirectFils(parent, process);

    printf("\n\t !! -- info : process created with PID = %d successfully  -- !!\n", process->PID);
}


void launchProcess(ProcessesTree root, CPU cpu){

    ProcessesTree process = getLowestPriority(root, HIGHEST_PRIORITY);

    printf("\n\t -- info : process founded with nom =>  %s --\n", process->nom);

    if(process->Etat == Elu){
        printf("\n\t !! -- error : the process is already running ! -- !!  \n");
        return;
    }

    if( canLaunch(process, cpu) ){
        startLaunchingProcess(process, cpu);
    }else{
        printf("\n\t !! -- error : the run stack is full ! -- !!  \n");
        return;
    }
}


/**
 * @brief function to remove sous tree from the mmain tree and , from cpu
*/
void killProcess(ProcessesTree root, CPU cpu){

    char* inputBuffer = (char*) malloc( sizeof(char) * 50 );

    printf("\n\t-the nom of the process you would kill => ");

    gets(inputBuffer);

    if(strlen(inputBuffer) > 11 || strlen(inputBuffer) <= 0 ){
        printf("\n !! -- error : the nom must be between 1-11 character length -- !!  \n");
        return;
    }
    
    ProcessesTree process = findProcessBynom(root, inputBuffer);

    if(process == NULL){
        printf("\n\t !! -- error : no process with that nom -- !!  \n");
        return;
    }

    if( strcmp(process->nom, "root") == 0 ){
         printf("\n\t !! -- warning : can not kill root process, instead chose cancel program -- !!  \n");
        return;
    }

    printf("\n\t -- info : process founded with PID =>  %d --\n", process->PID);


    startKillingProcess(root, process, cpu);

    printf("process killed successfully");
}

void terminateProcess(ProcessesTree root, CPU cpu){

    char* inputBuffer = (char*) malloc( sizeof(char) * 50 );
    printf("\n\t-the nom of the process you want to terminate => ");

    gets(inputBuffer);

    if(strlen(inputBuffer) > 11 || strlen(inputBuffer) <= 0 ){
        printf("\n !! -- error : the nom must be between 1-11 character length -- !!  \n");
        return;
    }
    
    ProcessesTree process = findProcessBynom(root, inputBuffer);

    if(process == NULL){
        printf("\n\t !! -- error : no process with that nom -- !!  \n");
        return;
    }

    if( strcmp(process->nom, "root") == 0 ){
         printf("\n\t !! -- warning : can not terminate root process, instead chose cancel program -- !!  \n");
        return;
    }

    printf("\n\t -- info : process founded with PID =>  %d --\n", process->PID);

    if(process->Etat != Elu){
         printf("\n\t !! -- error : the process must be in running mode to terminate -- !!  \n");
        return;
    }

    startKillingProcess(root, process, cpu);

    printf("process terminated successfully");
}

void blockProcess(ProcessesTree root, CPU cpu){
    char* inputBuffer = (char*) malloc( sizeof(char) * 50 );
    printf("\n\t-the nom of the process you woud launch => ");

    gets(inputBuffer);

    if(strlen(inputBuffer) > 11 || strlen(inputBuffer) <= 0 ){
        printf("\n !! -- error : the nom must be between 1-11 character length -- !!  \n");
        return;
    }
    
    ProcessesTree process = findProcessBynom(root, inputBuffer);

    if(process == NULL){
        printf("\n\t !! -- error : no process with that nom -- !!  \n");
        return;
    }

    if( strcmp(process->nom, "root") == 0 ){
         printf("\n\t !! -- error : can not block root process -- !!  \n");
        return;
    }

    printf("\n\t -- info : process founded with PID =>  %d --\n", process->PID);



    if(process->Etat != Elu){
        printf("\n\t !! -- error : the process is not in running mode -- !!  \n");
        return;
    }

    startBlockingProcess(process, cpu);
}

void deblockProcess(ProcessesTree root, CPU cpu){
    char* inputBuffer = (char*) malloc( sizeof(char) * 50 );
    printf("\n\t-the nom of the process you woud deblock => ");

    gets(inputBuffer);

    if(strlen(inputBuffer) > 11 || strlen(inputBuffer) <= 0 ){
        printf("\n !! -- error : the nom must be between 1-11 character length -- !!  \n");
        return;
    }
    
    ProcessesTree process = findProcessBynom(root, inputBuffer);

    if(process == NULL){
        printf("\n\t !! -- error : no process with that nom -- !!  \n");
        return;
    }

    if( strcmp(process->nom, "root") == 0 ){
         printf("\n\t !! -- warning : can not deblock root process-- !!  \n");
        return;
    }

    printf("\n\t -- info : process founded with PID =>  %d --\n", process->PID);

    if(process->Etat != Blocke){
        printf("\n\t !! -- error : the process must be Blocke to deblock -- !!  \n");
        return;
    }


    startDeblockingProcess(process);

    printf("process deBlocke successfully");
}

/**
 * @brief function show processes tree recursuvly
 * @param ProcessesTree root the head of the tree
 * @param CPU cpu 
 * @return void
*/
void showProcesses(ProcessesTree root, CPU cpu){

    startShowingProcesses(root, 1);

}


void LaunchSGP(){
    RAM = initRam(); //


    CPU cpu = initCPU(4);
    

    ProcessesTree root = newProcess("root", 1, 1, Pret);

    root->PID = 0;

    startLaunchingProcess(root, cpu);

    
    
    bool stop = false;

    void* callbacks[7] = {
        &createProcess,
        &launchProcess,
        &terminateProcess,
        &blockProcess,
        &deblockProcess,
        &killProcess,
        &showProcesses
    };

    int choice = -1;

    void* getCallback(int index){
        return callbacks[index];
    }


    while( !stop ){
        displayChoices(); //
        choice = getChoice(7);

        

        if(choice == 0){
            exit(EXIT_SUCCESS);
            //startKillingProcess(root, root, cpu);
            return;
        }else{

            void (*callback)(ProcessesTree, CPU) = getCallback( choice - 1 );

            callback(root, cpu);
        }

        system("pause");
        system("cls");

    }
}