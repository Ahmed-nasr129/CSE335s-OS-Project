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
    static process* currentProcess = nullptr;
    static bool isProcessing = false;
    
    if (!isProcessing && !readyQueue.empty()) {
        auto it = std::min_element(readyQueue.begin(), readyQueue.end(), 
            [](const process& a, const process& b) {
                return a.getBurstTime() < b.getBurstTime();
            });
        
        currentProcess = &(*it);
        currentProcess->setStartTime(time);
        isProcessing = true;
        readyQueue.erase(it);
    }
    
    if (isProcessing && currentProcess) {
        setCurrentProcess(*currentProcess);
        currentProcess->setRemainingTime(currentProcess->getRemainingTime() - 1);
        
        if (currentProcess->getRemainingTime() == 0) {
            currentProcess->setCompletionTime(time + 1);
            getCompletedProcesses().push_back(*currentProcess);
            isProcessing = false;
            currentProcess = nullptr;
            
            calculateAverageWaitingTime();
            calculateAverageTurnAroundTime();
            calculateAverageResponseTime();
        }
    }
}