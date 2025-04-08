#include "../include/SchedulerBase.h"
#include <iostream>
#include <chrono>
#include <thread>


void SchedulerBase::sleepIfRealtime(int seconds, bool realtime) const {
    if (realtime) {
        this_thread::sleep_for(chrono::seconds(seconds));
    }
}

void SchedulerBase::addProcess(const process& p) {
    process newProcess = p;
    newProcess.remaining = p.burst;
    allProcesses.push_back(newProcess);
}

void SchedulerBase::execute(process& p, int runTime, bool realtime) {
    if (p.start == -1) {
        p.start = currentTime;
    }
    cout << "Executing " << p.name << " from " << currentTime 
         << " to " << currentTime + runTime << endl;
    sleepIfRealtime(runTime, realtime);
}

void SchedulerBase::printStats() const {
    cout << "\nScheduler Stats at Time " << currentTime << ":\n";
    cout << "Completed Processes: " << completedProcesses.size() << endl;
}

double SchedulerBase::calculateAverageWaitingTime() const {
    if (completedProcesses.empty()) return 0.0;
    
    double total = 0;
    for (const auto& p : completedProcesses) {
        int waiting_time = p.start - p.arrival;
        int last_end = p.start;
        
        for (const auto& period : p.executionPeriods) {
            waiting_time += period.first - last_end;
            last_end = period.second;
        }
        
        total += waiting_time;
    }
    return total / completedProcesses.size();
}

double SchedulerBase::calculateAverageTurnaroundTime() const {
    if (completedProcesses.empty()) return 0.0;
    
    double total = 0;
    for (const auto& p : completedProcesses) {
        total += (p.end - p.arrival);
    }
    return total / completedProcesses.size();
}

const vector<process>& SchedulerBase::getCompletedProcesses() const {
    return completedProcesses;
}

const vector<process>& SchedulerBase::getAllProcesses() const {
    return allProcesses;
}