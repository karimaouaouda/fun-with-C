#ifndef __USED_ARRAY_STRUCTURE
#define __USED_ARRAY_STRUCTURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_types.h"
#include <time.h>

void writeStudentsToFile(Student students[], int numStudents, const char* filename) {
    FILE* file = fopen(filename, "w"); // Open file in write mode

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Write each student's details to the file
    for (int i = 0; i < numStudents; ++i) {
        fprintf(file, "Matricule: %d nom: %s prenom: %s groupe: %d\n", students[i].matricule, students[i].nom, students[i].prenom, students[i].group);
        // Write other student details if available
    }

    fclose(file); // Close the file
    printf("Data written to file successfully!\n");
}

void printStudents(Student students[], int numStudents) {
    printf("Printing students:\n");
    for (int i = 0; i < numStudents; ++i) {
        printf("Student : %s - Matricule: %d\n", students[i].nom, students[i].matricule);
        // Print other student details if available
    }
}


Student* extractData(const char* filename, int* size, double* time){

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
    Student* students = (Student*) malloc(sizeof(Student)); //set with one student;

    while (!feof(file))
    { // lire vers la fin de fichier
        if (fgets(line, sizeof(char) * 200 , file) != NULL)
        {
            //we will read the n student so we to increament the count to 
            //make it equal to n
            count ++;

            //after that we will make the array bigger to fit the current struct also
            students = (Student*) realloc(students, sizeof(Student) * count);

            

            //read the line into the new student (the new student automatically it's index is (n - 1) or (count - 1 in our case))
            sscanf(line, "Matricule: %d nom: %s prenom: %s groupe: %d", &(students[count - 1].matricule), students[count - 1].nom, students[count - 1].prenom, &(students[count - 1].group) );
        
        }
    }

    //finish reading file so now we just need to store the size and time and return data

    *size = count;

    end = clock(); // Record the end time

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the time difference

    *time = cpu_time_used; //example
    fclose(file);

    printStudents(students, count);

    return students;

}

void write_array_to_file(Student* students, const char* filename){

}

// Bubble sort function for an array of Student based on matricule
void bubbleSort(Student* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].matricule > arr[j + 1].matricule) {
                // Swap elements if they are in the wrong order
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


// Selection sort function for an array of Student based on matricule
void selectionSort(Student arr[], int n) {
    int i, j, minIndex;
    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j].matricule < arr[minIndex].matricule) {
                minIndex = j;
            }
        }
        // Swap elements if needed
        if (minIndex != i) {
            Student temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}


// Insertion sort function for an array of Student based on matricule
void insertionSort(Student arr[], int n) {
    int i, j;
    Student key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key.matricule
        // to one position ahead of their current position
        while (j >= 0 && arr[j].matricule > key.matricule) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Function to swap two students
void swap(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for Quick Sort
int partition(Student arr[], int low, int high) {
    int pivot = arr[high].matricule;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].matricule < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quick Sort function for an array of Student based on matricule
void quickSort(Student arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}





double sortData(Data* data, int algorithm){
    clock_t start, end;
    double cpu_time_used;

    Student* students = (Student*) data->students;
    int n = data->size;

    start = clock(); // Record the start time

    switch(algorithm){
        case __BUBBLE_SORT_ALGO:
            bubbleSort(students, n);
            break;

        case __SELECTION_SORT_ALGO:
            selectionSort(students, n);
            break;

        case __INSERTION_SORT_ALGO:
            insertionSort(students, n);
            break;

        case __QUICK_SORT_ALGO:
            quickSort(students, 0, n-1);
            break;
    }

    writeStudentsToFile(students, n, _SORTED_FILE_);


    end = clock(); // Record the end time

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the time difference

    return cpu_time_used;


    return 2.0;

}




#endif

/* 
// Function to merge two subarrays of arr[]
void merge(Student arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    Student L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].matricule <= R[j].matricule) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge Sort function for an array of Student based on matricule
void mergeSort(Student arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

 */