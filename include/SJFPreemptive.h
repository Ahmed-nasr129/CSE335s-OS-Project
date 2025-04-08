#ifndef SJF_PREEMATIVE_H
#define SJF_PREEMATIVE_H

#include "SchedulerBase.h"
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct CompareRemainingTime {
    bool operator()(const process& a, const process& b) {
        if (a.remaining == b.remaining)
            return a.arrival > b.arrival;
        return a.remaining > b.remaining;
    }
};

class SJFPreemptive : public SchedulerBase {
private:
    priority_queue<process, vector<process>, CompareRemainingTime> readyQueue;
    vector<process> waitingList;
    
public:
    SJFPreemptive();
    void run(bool live, bool realtime = false) override;
    void updateQueues() override;
    
    const priority_queue<process, vector<process>, CompareRemainingTime>& getReadyQueue() const;
    const vector<process>& getWaitingList() const;
};

#endif