#ifndef __MYSTRUCTS__ //this is guard in C to learn more about it, plase visit jacob sorber video about it
#define __MYSTRUCTS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include "custom_types.h"

char** listTextFiles(const char* directoryPath, int* s) {
    char** files = (char**) malloc(sizeof(char*));
    int count = 0;

    DIR* directory;
    struct dirent* entry;

    directory = opendir(directoryPath);

    if (directory == NULL) {
        printf("Could not open directory\n");
        return NULL;
    }


    while ((entry = readdir(directory)) != NULL) {
        // Check if the file has a .txt extension
        size_t len = strlen(entry->d_name);
        if (len > 4 && strcmp(entry->d_name + len - 4, ".txt") == 0) {
            files = (char**) realloc(files, ++count * sizeof(char*));
            files[count - 1] = (char*) malloc(sizeof(char) * strlen(entry->d_name) + 1 );

            strcpy(files[count - 1] , entry->d_name);

        }
    }
    closedir(directory);
    *s = count;


    return files;
    

    
}

Data* makeDataObject(void* extracted_data, int type, int size){
    Data* data = (Data*) malloc(sizeof(data));
    data->students = extracted_data;
    data->structure = type;
    data->size = size;

    return data;
}



double HandleChoices(int pipeline[], double times[], int * s){

    //i know that the first argument is the structure information

    void* extracted_data;
    int size = -1;
    double time = -1;
    int selection = pipeline[0];

    if(selection == _EXTRACT_AS_ARRAY_){
        printf("array :) ");
        #include "array.h"
        extracted_data = extractData(_DATA_FILENAME_, &size, &time);

        Data* data = makeDataObject(extracted_data, pipeline[0], size);

        times[0] = time;


        times[1] = sortData(data, pipeline[1]);

    }

    if(selection == _EXTRACT_AS_LINKEDLIST_){ 
        printf("linked list :) ");
        #include "linkedlist.h"
        extracted_data = extractData(_DATA_FILENAME_, &size, &time);

        Data* data = makeDataObject(extracted_data, pipeline[0], size);

        times[0] = time;


        times[1] = sortData(data, pipeline[1]);

    }


    if( selection == _EXTRACT_AS_ARBRE_ ){
        printf("binary tree :) ");
        #include "arbre.h" 
        extracted_data = extractData(_DATA_FILENAME_, &size, &time);

        Data* data = makeDataObject(extracted_data, pipeline[0], size);

        times[0] = time;


        times[1] = sortData(data, pipeline[1]);
    }


    printf("%d === %d - %d", selection, _EXTRACT_AS_LINKEDLIST_, _EXTRACT_AS_ARBRE_);

    *s = size;

    return time;
    
}



void displayOptions(unsigned int step)
{
    switch (step)
    {
        case _CHOSE_FILE_:
        puts("what file you want to work in ?");
        break;
        case _EXTRACT_METHODS_:
            puts("how would you extract data from file ?");
            puts("1 - extract data to array");
            puts("2 - extract data to linked list");
            puts("3 - extract data to binary tree");
            break;
        case _SORT_ALGO_CHOSE_:
            puts("which sort algorithm you want to use ?");
            puts("1 - bubble sort");
            puts("2 - selection sort");
            puts("3 - insertion sort");
            puts("4 - quick sort");
            break;
    }
}


int getNumber(int min, int max){
    char* input = (char*) malloc(sizeof(char) * 100);
    int n;
    do{
        printf("your choice in range(%d - %d) =>", min, max);
        gets(input);
        n = atoi(input);
    }while(n < min || n > max);

    free(input);
    return n;
}

int getChoice(int steps, char** files, int files_count){
    switch(steps){
        case _CHOSE_FILE_:
            for(int i = 0 ; i < files_count ; i++){
                printf("%d - %s\n", i + 1 , files[i]);
            }
            return getNumber(1, files_count);
            break;
        case _EXTRACT_METHODS_:
            return getNumber(1, 3);
            break;
        case _SORT_ALGO_CHOSE_:
            return getNumber(1, 4);
            break;
    }
}


void generateRapport(int pipeline[], double times[], int size){
    char extract_methods[][12] = {
        "array",
        "linked list",
        "binary tree"
    };

    char sort_algo[][10] = {
        "bubble",
        "selection",
        "insertion",
        "quick"
    };

    FILE* file = fopen(_RAPPORT_FILENAME_, "w");

    if(!file){
        fprintf(stderr, "failed to open stream, no such file %s", _RAPPORT_FILENAME_);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "- all record that fetched : %d\n- the structur used : %s (%f s)\n- the sort algoritm used : %s (%f)\n- the total time taken : %f",size, extract_methods[pipeline[0] - 1], times[0] , sort_algo[ pipeline[1] - 1 ], times[1] , ( times[0] + times[1] ) );

    fclose(file);

    puts("rapport generated successfully");

}



void startProgram()
{
    unsigned int choice;
    unsigned int steps = 0;
    bool chosing = true;
    double time = 0;
    int files_count = -1;

    char** file_list = listTextFiles("./data", &files_count);

    if(files_count <= 0){
        fprintf(stderr, "sorry there is no files in %s folder", _DATA_FOLDER_);
        exit(EXIT_FAILURE);
    }


    double times[__PIPELINE__SIZE__] = {-1, -1};

    int pipeline[__PIPELINE__SIZE__] = {-1, -1};

    // in this function we need the etaps
    // 1 - the user see a list of option, he must chose from first how to extract data from file
    while (steps < (__PIPELINE__SIZE__ + 1)  )
    {
        displayOptions(steps);

        choice = getChoice(steps,file_list, files_count);

        if(steps == _CHOSE_FILE_){
            strcat(_DATA_FILENAME_, _DATA_FOLDER_);
            strcat(_DATA_FILENAME_, "/");
            strcat(_DATA_FILENAME_, file_list[ choice - 1 ]);
            steps++;
            continue;
        }

        pipeline[steps-1] = choice;

        steps++;
    }

    int size;

    //end of the choices, now we will execute the program with the options
    HandleChoices(pipeline, times, &size);

    generateRapport(pipeline, times, size);
}


#endif