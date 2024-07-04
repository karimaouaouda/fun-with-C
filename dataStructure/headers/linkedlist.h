#ifndef __USED_LINKEDLIST_STRUCTURE
#define __USED_LINKEDLIST_STRUCTURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_types.h"
#include <time.h>

void writeStudentsToFile(List* head, const char* filename){
    FILE* file = fopen(filename, "w"); // Open file in write mode

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    List* temp = head;
    while(temp != NULL){
        fprintf(file, "Matricule: %d nom: %s prenom: %s groupe: %d\n", temp->data->matricule, temp->data->nom, temp->data->prenom, temp->data->group);
        temp = temp->next;
    }
}


List* createListNode(Student* student){
    List* node = (List*) malloc(sizeof(List));

    node->data = student;
    node->next = NULL;

    return node;
}

List* insertIntoList(List* list, Student* student){
    if(list == NULL){
        List* node = createListNode(student);
        return node;
    }

    list->next = insertIntoList(list->next, student);

    return list;
}


// Function to perform bubble sort on the linked list
void bubbleSort(List* head) {
    int swapped;
    List* ptr;
    List* lptr = NULL;

    if (head == NULL)
        return;

    do {
        swapped = 0;
        ptr = head;

        while (ptr->next != lptr) {
            if (ptr->data->matricule > ptr->next->data->matricule) {
                Student *temp = ptr->data;
                ptr->data = ptr->next->data;
                ptr->next->data = temp;
                swapped = 1;
            }
            ptr = ptr->next;
        }
        lptr = ptr;
    } while (swapped);
}


// Function to perform insertion sort on the linked list
void insertionSort(List** headRef) {
    List* sorted = NULL;
    List* current = *headRef;

    while (current != NULL) {
        List* next = current->next;

        // Find the position to insert current node in the sorted list
        List** sortedRef = &sorted;
        while (*sortedRef != NULL && (*sortedRef)->data->matricule < current->data->matricule) {
            sortedRef = &((*sortedRef)->next);
        }

        // Insert the current node into the sorted list
        current->next = *sortedRef;
        *sortedRef = current;

        current = next;
    }

    // Update the head pointer to point to the sorted list
    *headRef = sorted;
}

// Function to swap data of two nodes
void swap(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

// Function to perform selection sort on the linked list
void selectionSort(List* head) {
    List* current = head;

    while (current != NULL) {
        List* min = current;
        List* temp = current->next;

        while (temp != NULL) {
            if (temp->data->matricule < min->data->matricule) {
                min = temp;
            }
            temp = temp->next;
        }

        // Swap the data of nodes if needed
        swap((min->data), (current->data));
        current = current->next;
    }
}



// Function to find the last node of the linked list
List* lastNode(List* node) {
    while (node && node->next) {
        node = node->next;
    }
    return node;
}

// Partition function for Quick Sort
List* partition(List* low, List* high, List** newLow, List** newHigh) {
    Student pivot = *(high->data);
    List* i = low->next;
    List* prev = low;

    while (i != high) {
        if (i->data->matricule < pivot.matricule) {
            swap((prev->data), (i->data));
            prev = prev->next;
        }
        i = i->next;
    }
    swap((prev->data), (high->data));
    *newLow = low;
    *newHigh = prev;

    return prev;
}

// Quick Sort function for a linked list
void quickSort(List* low, List* high) {
    if (high != NULL && low != high && low != high->next) {
        List *newLow, *newHigh;
        List* pivot = partition(low, high, &newLow, &newHigh);

        quickSort(low, pivot);
        quickSort(pivot->next, high);
    }
}

// Function to print the linked list of students
void printLinkedList(List* head) {
    while (head != NULL) {
        printf("Matricule: %d\n", head->data->matricule);
        // Print other student details if available
        head = head->next;
    }
}


// Function to merge two sorted linked lists
List* merge(List* left, List* right) {
    List dummy;
    List* tail = &dummy;
    dummy.next = NULL;

    while (left != NULL && right != NULL) {
        if (left->data->matricule <= right->data->matricule) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    return dummy.next;
}

// Function to get the middle node of the linked list
List* getMiddle(List* head) {
    if (head == NULL) {
        return head;
    }

    List* slow = head;
    List* fast = head->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    return slow;
}

// Merge Sort function for a linked list
List* mergeSortLinkedList(List* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    List* middle = getMiddle(head);
    List* nextToMiddle = middle->next;
    middle->next = NULL;

    List* left = mergeSortLinkedList(head);
    List* right = mergeSortLinkedList(nextToMiddle);

    return merge(left, right);
}

List* extractData(const char* filename, int* size, double* time){

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE* file = fopen(filename, "r");

    if(!file){
        fprintf(stderr, "failed to open stream :  no such file named %s", filename);
        exit(EXIT_FAILURE);
    }

    int count = 0; //count of the students to read
    char* line =  (char*) malloc(sizeof(char) * 200); //we assume that at most the line has 200 character
    List* list = NULL;

    Student* student; // variable to store every time the student adress (new one)

    while (!feof(file))
    { // lire vers la fin de fichier
        if (fgets(line, sizeof(char) * 200, file) != NULL)
        {
            //we will read the n student so we to increament the count to 
            //make it equal to n
            count ++;

            student = (Student*) malloc(sizeof(Student));

            //read the line into the new student (the new student automatically it's index is (n - 1) or (count - 1 in our case))
            sscanf(line, "Matricule: %d nom: %s prenom: %s groupe: %d", &(student->matricule), student->nom, student->prenom, &(student->group) );
        
            list = insertIntoList(list, student);

            printf("d");
        
        }
    }


    *size = count;

    end = clock(); // Record the end time

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the time difference

    *time = cpu_time_used; //example
    fclose(file);


    return list;
}


double sortData(Data* data, int algorithm){
    clock_t start, end;
    double cpu_time_used;

    List* students = data->students;

    start = clock(); // Record the start time

    switch(algorithm){
        case __BUBBLE_SORT_ALGO:
            bubbleSort(students);
            break;

        case __SELECTION_SORT_ALGO:
            selectionSort(students);
            break;

        case __INSERTION_SORT_ALGO:
            insertionSort(&students);
            break;

        case __QUICK_SORT_ALGO:
            quickSort(students, lastNode(students));
            break;
    }

    end = clock(); // Record the end time

    writeStudentsToFile(students, _SORTED_FILE_);

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the time difference

    return cpu_time_used;
}

#endif