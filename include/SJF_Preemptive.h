#ifndef SJF_PREEMPTIVE_H
#define SJF_PREEMPTIVE_H

#include "Scheduler.h"
#include <vector>
#include <algorithm>

class SJF_Preemptive : public Scheduler {
private:
    vector<process> readyQueue;

public:
    SJF_Preemptive();
    void addProcess(process p, int time) override;
    void beginOperation(int time) override;
};

#endif // SJF_PREEMPTIVE_H