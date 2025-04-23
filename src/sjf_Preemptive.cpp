#include "../include/SJF_Preemptive.h"


SJF_Preemptive::SJF_Preemptive() : Scheduler() {
    
}

void SJF_Preemptive::addProcess(process p, int time) {
    p.setArrivalTime(time);
    readyQueue.push_back(p);
}

void SJF_Preemptive::beginOperation(int time) {
    if (!readyQueue.empty()) {
        // Find the process with the shortest remaining time
        auto it = min_element(readyQueue.begin(), readyQueue.end(), 
            [](const process& a, const process& b) {
                return a.getRemainingTime() < b.getRemainingTime();
            });
        
        process& current = *it;
        
        if (current.getRemainingTime() == current.getBurstTime()) {
            current.setStartTime(time);
        }
        

        current.setRemainingTime(current.getRemainingTime() - 1);

        if (current.getRemainingTime() == 0) {
            current.setCompletionTime(time + 1); // +1 because it completes at the end of this time unit
            getCompletedProcesses().push_back(current);
            readyQueue.erase(it);
        }
        setCurrentProcess(current);
    }
}
