#ifndef SJF_NONPREEMPTIVE_H
#define SJF_NONPREEMPTIVE_H

#include "Scheduler.h"
#include <algorithm>
#include <vector>

class SJF_NonPreemptive : public Scheduler {
private:
    std::vector<process> readyQueue;

public:
    SJF_NonPreemptive();  // Declaration only
    void addProcess(process p, int time) override;
    void beginOperation(int time) override;
};

#endif // SJF_NONPREEMPTIVE_H