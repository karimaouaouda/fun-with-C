#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"


Process *loadProcesses(int *len);
Process processStartAt(Process *processes, int size, int time);
void executeCPU(Process *processes, int processes_count)
{
    int time = 0; // time starts at 0
    List queue = NULL;
    CPU cpu = newCPU();

    freeLogFile();

    while (true)
    {
        

        if( ! canContinue(cpu, queue, processes, processes_count) ){
            printf("\n\tfinished\n");
            break;
        }
        

        Process arrive = processStartAt(processes, processes_count, time);

        if (arrive != NULL)
        {
            // processes arrive at time
            
            if (getExecuteTime(arrive) < 1)
            {
                log(arrive, time);
                continue;
            }

            queue = addProcess(queue, cpu, arrive);

            printList(queue);
            printf("arrived : %d in time : %d\n", getId(arrive), time);
            int status = executeProcess(cpu, 1);

            if(status == _PROCESS_FINISH_){
                log(getCurrentRunningProcess(cpu), time + 1);
                freeCpu(cpu);
                choseAnotherone(&queue, cpu);
            }

            time++;
            _sleep(100);
        }else{

            printList(queue);
            if( ! isCpuEmpty(cpu) ){
                printf("\ntime : %d\n", time);
                int status = executeProcess(cpu, 1);

                if(status == _PROCESS_FINISH_){
                    log(getCurrentRunningProcess(cpu), time + 1);
                    freeCpu(cpu);
                    choseAnotherone(&queue, cpu);
                }
            }
            
            time++;
            _sleep(100);
        }

        
    }
}

int main()
{
    int size = 0;

    Process *processes = loadProcesses(&size);

    executeCPU(processes, size);

    return 0;
}

Process *loadProcesses(int *len)
{
    FILE *file = fopen(__FILENAME__, "r");
    Process *Processes = NULL;
    int length = 0;

    if (file == NULL)
    {
        fprintf(stderr, "can not load file \"%s\", verify the path to file", __FILENAME__);
        exit(EXIT_FAILURE);
    }

    char *line = malloc(sizeof(char) * 20);

    positive id, arriveat, exectime, priority;

    while (fgets(line, 19, file))
    {
        sscanf(line, "%u,%u,%u,%u", &id, &arriveat, &exectime, &priority);

        if (Processes == NULL)
        {
            Processes = malloc(sizeof(Process));
            length = 1;
        }
        else
        {
            Processes = realloc(Processes, sizeof(Process) * (length + 1));
            length++;
        }

        // we know that the index must be length - 1

        Processes[length - 1] = newProcess(id, exectime, arriveat, priority);
    }

    *len = length;
    return Processes;
}

Process processStartAt(Process *processes, int size, int time)
{
    for (int i = 0; i < size; i++)
    {
        if (processes[i] != NULL)
        {

            if (getArriveTime(processes[i]) == time)
            {
                Process temp = processes[i];
                processes[i] = NULL;

                return temp;
            }
        }
    }

    return NULL;
}