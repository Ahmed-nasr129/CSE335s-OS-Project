#include "../include/SJFNonPreemptive.h"
#include <iostream>

SJFNonPreemptive::SJFNonPreemptive() {}

void SJFNonPreemptive::run(bool live, bool realtime) {
    currentTime = 0;
    
    while (!allProcesses.empty() || !waitingList.empty()) {
        updateQueues();
        
        if (waitingList.empty()) {
            currentTime++;
            continue;
        }
        
        process current = waitingList.front();
        waitingList.erase(waitingList.begin());
        
        current.start = currentTime;
        execute(current, current.burst, realtime);
        currentTime += current.burst;
        
        current.end = currentTime;
        current.finished = true;
        completedProcesses.push_back(current);
        
        if (live) {
            printStats();
        }
    }
}

void SJFNonPreemptive::updateQueues() {
    // Move arrived processes from allProcesses to waitingList
    for (auto it = allProcesses.begin(); it != allProcesses.end(); ) {
        if (it->arrival <= currentTime) {
            waitingList.push_back(*it);
            it = allProcesses.erase(it);
        } else {
            ++it;
        }
    }
    
    // Sort waitingList by burst time (SJF)
    sort(waitingList.begin(), waitingList.end(), 
        [](const process& a, const process& b) {
            return a.burst < b.burst;
        });
}

const priority_queue<process, vector<process>, CompareBurstTime>& 
SJFNonPreemptive::getReadyQueue() const {
    return readyQueue;
}

const vector<process>& SJFNonPreemptive::getWaitingList() const {
    return waitingList;
}