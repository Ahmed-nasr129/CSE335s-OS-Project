#include "Scheduler.h"

Scheduler::Scheduler(){
    averageWaitingTime = 0;
    averageTurnAroundTime = 0;
    averageResponseTime = 0;
}

void Scheduler::calculateAverageWaitingTime() {
    int temp = 0;
    for (process completedProcess : completedProcesses) {
        temp +=
            completedProcess.turnAroundTime - completedProcess.burstTime;
    }
    averageWaitingTime = temp/(double)(completedProcesses.size());
}

void Scheduler::calculateAverageTurnAroundTime() {
    int temp = 0;
    for (process completedProcess : completedProcesses) {
        temp += completedProcess.turnAroundTime =
            completedProcess.completionTime-completedProcess.arrivalTime;
    }
    averageTurnAroundTime = temp/(double)(completedProcesses.size());
}

void Scheduler::calculateAverageResponseTime() {
    int temp = 0;
    for (process completedProcess : completedProcesses) {
        temp +=
            completedProcess.startTime - completedProcess.arrivalTime;
    }
    averageResponseTime = temp/(double)(completedProcesses.size());
}

double Scheduler::getAverageResponseTime() const {
    return averageResponseTime;
}

double Scheduler::getAverageWaitingTime() const {
    return averageWaitingTime;
}

double Scheduler::getAverageTurnAroundTime() const {
    return averageTurnAroundTime;
}

process Scheduler::getCurrentProcess() const {
    return currentProcess;
}

void Scheduler::setCurrentProcess(process process) {
    currentProcess = process;
}

vector<process>& Scheduler::getCompletedProcesses()  {
    return completedProcesses;
}







