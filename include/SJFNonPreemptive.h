#ifndef SJF_NON_PREEMPTIVE_H
#define SJF_NON_PREEMPTIVE_H

#include "SchedulerBase.h"
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct CompareBurstTime {
    bool operator()(const process& a, const process& b) {
        if (a.burst == b.burst)
            return a.arrival > b.arrival;
        return a.burst > b.burst;
    }
};

class SJFNonPreemptive : public SchedulerBase {
private:
    priority_queue<process, vector<process>, CompareBurstTime> readyQueue;
    vector<process> waitingList;
    
public:
    SJFNonPreemptive();
    void run(bool live, bool realtime = false) override;
    void updateQueues() override;
    
    const priority_queue<process, vector<process>, CompareBurstTime>& getReadyQueue() const;
    const vector<process>& getWaitingList() const;
};

#endif