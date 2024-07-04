#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"

struct ProcessStruct
{
    positive ID;
    positive arrive_at;
    positive exec_time;
    positive priority;
    positive finish_at;
};

struct ListStruct
{
    Process process;
    positive size;
    struct ListStruct *next;
};

struct CPUStruct
{
    Process current_process;
};

CPU newCPU()
{
    CPU cpu = malloc(sizeof(struct CPUStruct));
    ;
    cpu->current_process = NULL;
    return cpu;
}

// helpers
bool isCpuEmpty(CPU cpu)
{
    return cpu->current_process == NULL;
}

positive getExecuteTime(Process p)
{
    return p->exec_time;
}

positive getArriveTime(Process p)
{
    return p->arrive_at;
}

List newList(Process p)
{
    List l = malloc(sizeof(List));
    l->process = p;
    l->size = 1;
    l->next = NULL;

    return l;
}

List unshift(List list, Process process)
{
    List nl = newList(process);

    if (list == NULL)
    {
        return nl;
    }

    nl->next = list;

    nl->size += list->size;

    return nl;
}

Process shift(List *list)
{
    // get the first process in list

    List temp = (*list)->next;

    Process p = (*list)->process;

    (*list)->size--;

    *list = temp;

    return p;
}

List findBestPlaceFor(Process process, List list)
{

    List temp = list;
    List next = NULL;
    List prev = next;
    while (temp->next != NULL)
    {
        prev = temp;
        next = temp->next;

        if (process->priority <= next->process->priority)
        {
            return prev;
        }

        temp = temp->next;
    }

    return temp;
}

List insertInList(List list, Process process)
{
    if(list == NULL){
        return newList(process);
    }
    if (process->priority <= list->process->priority)
    {
        return unshift(list, process);
    }

    List l = findBestPlaceFor(process, list);

    if (l->next == NULL)
    {
        l->next = newList(process);

        return list;
    }

    List nl = newList(process);

    nl->next = l->next;
    l->next = nl;

    list->size++;

    return list;
}

Process newProcess(positive id, positive exec_time, positive arrive_at, positive priority)
{
    Process p = malloc(sizeof(struct ProcessStruct));
    p->arrive_at = arrive_at;
    p->exec_time = exec_time;
    p->ID = id;
    p->priority = priority;

    return p;
}

positive getId(Process p)
{
    return p->ID;
}

void printList(List list)
{
    if (list == NULL)
    {
        printf("null\n");
    }
    else
    {
        printf("%d => ", list->process->ID);
        printList(list->next);
    }
}

List addProcess(List list, CPU cpu, Process process)
{
    if (isCpuEmpty(cpu))
    {
        cpu->current_process = process;
        return list;
    }

    if (process->priority < cpu->current_process->priority)
    {
        Process temp = cpu->current_process;
        cpu->current_process = process;

        return unshift(list, temp);
    }

    return insertInList(list, process);
}

int executeProcess(CPU cpu, int time)
{

    printf("executing process %d (%d)\n", cpu->current_process->ID, cpu->current_process->exec_time);

    cpu->current_process->exec_time -= time;

    if (cpu->current_process->exec_time == 0)
    {
        return _PROCESS_FINISH_;
    }

    return _PROCESS_STILL_;
}

Process getCurrentRunningProcess(CPU cpu)
{
    return cpu->current_process;
}

void freeCpu(CPU cpu)
{
    printf("free cpu ...\n");
    cpu->current_process = NULL;
}

bool emptyList(List list)
{
    if (list == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool canContinue(CPU cpu, List queue, Process *processes, int size)
{
    bool empty_array, empty_queue, empty_cpu;

    empty_array = empty_queue = empty_cpu = false;

    for (int i = 0; i < size; i++)
    {
        if (processes[i] != NULL)
        {
            return true;
        }
    }

    if (!emptyList(queue))
    {
        return true;
    }

    if (!isCpuEmpty(cpu))
    {
        return true;
    }

    return false;
}

void choseAnotherone(List *queue, CPU cpu)
{
    printf("chosing another ... \n");
    if ((*queue) == NULL)
    {
        return;
    }
    cpu->current_process = shift(queue);
}

void freeLogFile()
{
    FILE *file = fopen(_LOG_FILENAME_, "w");

    if (!file)
    {

        fprintf(stderr, "error counting where opening %s file", _LOG_FILENAME_);

        return;
    }

    fprintf(file, "");
}

void log(Process p, int time)
{
    if (p != NULL)
    {

        FILE *file = fopen(_LOG_FILENAME_, "a");

        if (!file)
        {

            fprintf(stderr, "error counting where opening %s file", _LOG_FILENAME_);

            return;
        }

        p->finish_at = time;

        fprintf(file, TEMPLATE, p->ID, p->arrive_at, (p->finish_at - p->arrive_at));

        fclose(file);
    }
}