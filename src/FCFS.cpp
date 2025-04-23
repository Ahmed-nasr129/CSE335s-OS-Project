#include "../include/FCFS.h"

FCFS_Scheduler::FCFS_Scheduler(){}

void FCFS_Scheduler::addProcess(process p, int time) {
    p.setArrivalTime(time);
    processQueue.push(p);
}

void FCFS_Scheduler::beginOperation(int time) {
    if (!processQueue.empty()) {
        if(!isrunning){
            currentProcess = processQueue.front();
            isrunning=1;
        }
        if (currentProcess.getRemainingTime() == currentProcess.getBurstTime()) {currentProcess.setStartTime(time);}
        currentProcess.setRemainingTime(currentProcess.getRemainingTime()-1);
        if (currentProcess.getRemainingTime() == 0) {
            processQueue.pop();
            currentProcess.setCompletionTime(time+1);
            getCompletedProcesses().push_back(currentProcess);
            isrunning=0;
        }
    }
}



