#include "../include/priorityNonPreemtive.h"

priority_queue<process,vector<process>,priorityNonPreemtive::compare> priorityNonPreemtive::get_ready_qeueue() const {
    return readyQeueue;
}

void priorityNonPreemtive::addProcess(process p, int time) {
    p.setArrivalTime(time);
    readyQeueue.push(p);
}

void priorityNonPreemtive::beginOperation(int time) {
    if(currentProcess.getRemainingTime() <= 0) {
        if(!readyQeueue.empty()) {
            currentProcess = readyQeueue.top();
            readyQeueue.pop();
            if(currentProcess.getRemainingTime() == currentProcess.getBurstTime())
                currentProcess.setStartTime(time);
            currentProcess.setRemainingTime(currentProcess.getRemainingTime()-1);
            if(currentProcess.getRemainingTime() <= 0) {
                currentProcess.setCompletionTime(time+1);
                completedProcesses.push_back(currentProcess);
            }
        }
    }else {
        if(currentProcess.getRemainingTime() == currentProcess.getBurstTime())
            currentProcess.setStartTime(time);
        currentProcess.setRemainingTime(currentProcess.getRemainingTime()-1);
        if(currentProcess.getRemainingTime() <= 0) {
            currentProcess.setCompletionTime(time+1);
            completedProcesses.push_back(currentProcess);
        }
    }
}
