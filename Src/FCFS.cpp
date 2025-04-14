#include "FCFS.h"

void FCFS_Scheduler::addProcess(process p, int time) {
    p.setArrivalTime(time);
    processQueue.push(p);
}

void FCFS_Scheduler::beginOperation(int time) {
    if (!processQueue.empty()) {
        process current = processQueue.front();
        if (current.getRemainingTime() == current.getBurstTime()) {current.setStartTime(time);}
        setCurrentProcess(current);
        current.setRemainingTime(current.getRemainingTime()-1);
        if (current.getRemainingTime() == 0) {
            processQueue.pop();
            current.setCompletionTime(time);
            getCompletedProcesses().push_back(current);
        }
    }
}



