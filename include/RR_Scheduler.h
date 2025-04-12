//
// Created by Jana Aly on 4/9/2025.
//
#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H
#include "Scheduler.h"
#pragma once
class RR_Scheduler : public Scheduler {
    //attributes
private:
    //process inCPU;
    queue<process>readyQueue;
    int decreased=0;
    int qt;
    int cnt=0;
    struct Sorter {
        bool operator()(const process& a, const process& b) {
            return a.getArrivalTime()>b.getArrivalTime();
        }
    };

public:
    RR_Scheduler(int qt);
    void setQT(int qt);
    void addProcess(process p,int time) override;
    void beginOperation(int time) override;
    void roundRobin(int time);
    void updateQueue(int qt);
    void ToReadyQueue();
};
#endif //RR_SCHEDULER_H
