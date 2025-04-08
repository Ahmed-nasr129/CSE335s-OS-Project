#include "../include/SJFPreemptive.h"
#include <iostream>


SJFPreemptive::SJFPreemptive() {}

void SJFPreemptive::run(bool live, bool realtime) {
    currentTime = 0;
    
    while (!allProcesses.empty() || !waitingList.empty()) {
        updateQueues();
        
        if (waitingList.empty()) {
            currentTime++;
            continue;
        }
        
        process& current = waitingList.front();
        
        if (current.start == -1) {
            current.start = currentTime;
        }
        
        // Record execution start
        current.executionPeriods.emplace_back(currentTime, currentTime + 1);
        
        execute(current, 1, realtime);
        currentTime++;
        current.remaining--;
        
        if (current.remaining <= 0) {
            current.end = currentTime;
            current.finished = true;
            completedProcesses.push_back(current);
            waitingList.erase(waitingList.begin());
        }
        
        if (live) {
            printStats();
        }
    }
}

void SJFPreemptive::updateQueues() {
    // Move arrived processes from allProcesses to waitingList
    for (auto it = allProcesses.begin(); it != allProcesses.end(); ) {
        if (it->arrival <= currentTime) {
            waitingList.push_back(*it);
            it = allProcesses.erase(it);
        } else {
            ++it;
        }
    }
    
    // Sort waitingList by remaining time (SJF Preemptive)
    sort(waitingList.begin(), waitingList.end(), 
        [](const process& a, const process& b) {
            return a.remaining < b.remaining;
        });
}

const priority_queue<process, vector<process>, CompareRemainingTime>& 
SJFPreemptive::getReadyQueue() const {
    return readyQueue;
}

const vector<process>& SJFPreemptive::getWaitingList() const {
    return waitingList;
}