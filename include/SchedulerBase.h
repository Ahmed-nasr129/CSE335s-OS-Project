#ifndef SCHEDULER_BASE_H
#define SCHEDULER_BASE_H

#include "Process.h"
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

class SchedulerBase {
protected:
    int currentTime = 0;
    vector<process> allProcesses;
    vector<process> completedProcesses;
    
    void execute(process& p, int runTime, bool realtime);
    void printStats() const;
    void sleepIfRealtime(int seconds, bool realtime) const;
    
public:
    virtual ~SchedulerBase() = default;
    void addProcess(const process& p);
    virtual void run(bool live, bool realtime = false) = 0;
    virtual void updateQueues() = 0;
    
    double calculateAverageWaitingTime() const;
    double calculateAverageTurnaroundTime() const;
    
    const vector<process>& getCompletedProcesses() const;
    const vector<process>& getAllProcesses() const;
};

#endif