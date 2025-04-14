#include "../include/priorityPreemtive.h"

priority_queue<process,vector<process>,priorityPreemtive::compare> priorityPreemtive::get_ready_qeueue() const {
    return readyQeueue;
}

void priorityPreemtive::addProcess(process p, int time) {
    p.setArrivalTime(time);
    readyQeueue.push(p);
}

void priorityPreemtive::beginOperation(int time) {
    if(!readyQeueue.empty()) {
        currentProcess = readyQeueue.top();
        if(currentProcess.getRemainingTime() == currentProcess.getBurstTime())
            currentProcess.setStartTime(time);
        currentProcess.setRemainingTime(currentProcess.getRemainingTime()-1);
        readyQeueue.pop();
        readyQeueue.push(currentProcess);
        if(currentProcess.getRemainingTime() <= 0) {
            currentProcess.setCompletionTime(time+1);
            completedProcesses.push_back(currentProcess);
            readyQeueue.pop();
        }
    }
}
