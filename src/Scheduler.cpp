#include "../include/Scheduler.h"

// Process methods implementation
void process::setRemainingTime(int remaining) { remainingTime = remaining; }
void process::setCompletionTime(int completion) { completionTime = completion; }
void process::setArrivalTime(int arrival) { arrivalTime = arrival; }
void process::setStartTime(int start) { startTime = start; }

int process::getRemainingTime() const { return remainingTime; }
int process::getCompletionTime() const { return completionTime; }
int process::getBurstTime() const { return burstTime; }
int process::getPid() const { return pid; }
int process::getArrivalTime() const { return arrivalTime; }
int process::getStartTime() const { return startTime; }
int process::getPriority() const { return priority; }

// Scheduler methods implementation
Scheduler::Scheduler() : averageWaitingTime(0), averageTurnAroundTime(0), averageResponseTime(0) {}
Scheduler::~Scheduler() {}

void Scheduler::calculateAverageWaitingTime() {
    double total = 0;
    for (const auto& p : completedProcesses) {
        total += (p.getCompletionTime() - p.getArrivalTime() - p.getBurstTime());
    }
    averageWaitingTime = completedProcesses.empty() ? 0 : total / completedProcesses.size();
}

void Scheduler::calculateAverageTurnAroundTime() {
    double total = 0;
    for (const auto& p : completedProcesses) {
        total += (p.getCompletionTime() - p.getArrivalTime());
    }
    averageTurnAroundTime = completedProcesses.empty() ? 0 : total / completedProcesses.size();
}

void Scheduler::calculateAverageResponseTime() {
    double total = 0;
    for (const auto& p : completedProcesses) {
        total += (p.getStartTime() - p.getArrivalTime());
    }
    averageResponseTime = completedProcesses.empty() ? 0 : total / completedProcesses.size();
}

double Scheduler::getAverageWaitingTime() const { return averageWaitingTime; }
double Scheduler::getAverageTurnAroundTime() const { return averageTurnAroundTime; }
double Scheduler::getAverageResponseTime() const { return averageResponseTime; }

void Scheduler::setCurrentProcess(process p) { currentProcess = p; }
process Scheduler::getCurrentProcess() const { return currentProcess; }
vector<process>& Scheduler::getCompletedProcesses() { return completedProcesses; }