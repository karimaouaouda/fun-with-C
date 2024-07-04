#ifndef __CPU_HEADER__
#define __CPU_HEADER__

#include <stdbool.h>

#define _PROCESS_FINISH_ 0x2
#define _PROCESS_STILL_ 0x4
#define TEMPLATE "\nprocess %d :\n\tarrive time : %d\n\ttemp de reponse %d "

#define __FILENAME__ "processes.txt"
#define _LOG_FILENAME_ "log.txt"


typedef unsigned int positive;

typedef struct ProcessStruct* Process;
typedef struct CPUStruct* CPU;
typedef struct ListStruct* List;


Process newProcess(positive id, positive exec_time, positive arrive_at, positive priority);

positive getId(Process);

CPU newCPU();

bool isCpuEmpty(CPU cpu);

positive getExecuteTime(Process);

positive getArriveTime(Process);

int executeProcess(CPU cpu, int time);

List addProcess(List list, CPU cpu, Process process);

void printList(List list);

Process getCurrentRunningProcess(CPU cpu);

void freeCpu(CPU cpu);

bool canContinue(CPU, List, Process*, int);

void choseAnotherone(List*, CPU);

void log(Process p, int time);

void freeLogFile();




#endif