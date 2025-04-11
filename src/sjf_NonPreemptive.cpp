#include "../include/SJF_NonPreemptive.h"

// Implementation of constructor
SJF_NonPreemptive::SJF_NonPreemptive() : Scheduler() {
    // Constructor body (can be empty)
}

void SJF_NonPreemptive::addProcess(process p, int time) {
    p.setArrivalTime(time);
    readyQueue.push_back(p);
}

void SJF_NonPreemptive::beginOperation(int time) {
    // If no process is currently running, select the next one
    if (!currentProcess.getPid()) {
        if (!readyQueue.empty()) {
            // Find process with shortest burst time that has arrived
            auto it = min_element(readyQueue.begin(), readyQueue.end(),
                [](const process& a, const process& b) {
                    return a.getBurstTime() < b.getBurstTime();
                });
            
            currentProcess = *it;
            currentProcess.setStartTime(time);
            readyQueue.erase(it);
        }
    }
    
    // If we have a current process, execute it
    if (currentProcess.getPid()) {
        setCurrentProcess(currentProcess);
        currentProcess.setRemainingTime(currentProcess.getRemainingTime() - 1);
        
        // Check if process completed
        if (currentProcess.getRemainingTime() == 0) {
            currentProcess.setCompletionTime(time + 1);
            getCompletedProcesses().push_back(currentProcess);
            currentProcess = process(); // Reset to empty process
        }
    }
}