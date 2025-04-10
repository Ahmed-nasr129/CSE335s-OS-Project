#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <string>
#include <vector>
using namespace std;
class process{
private:
    int pid;
    int arrivalTime;
    int priority; // -1 in case of no priority
    int burstTime;
    int startTime;
    int completionTime;
    int remainingTime;
    int turnAroundTime;

    process();

public:
    process(int pid, int arrival, int burst, int priority = -1) {
        this->pid = pid;
        this->arrivalTime = arrival;
        this->burstTime = burst;
        this->priority = priority;
        this->remainingTime = burst;
    }

    void setRemainingTime(int remaining) {
        this->remainingTime = remaining;
    }

    void setCompletionTime(int completion) {
        this->completionTime = completion;
    }

    void setArrivalTime(int arrival) {
        this->arrivalTime = arrival;
    }

    void setStartTime(int start) {
        this->startTime = start;
    }

    int getRemainingTime() const {
        return remainingTime;
    }

    int getCompletionTime() const {
        return completionTime;
    }

    int getBurstTime() const {
        return burstTime;
    }

    friend class Scheduler;
};

class Scheduler {
private:
    process currentProcess;
    vector<process> completedProcesses;
    double averageWaitingTime{};
    double averageTurnAroundTime{};
    double averageResponseTime;

public:
    Scheduler();
    //3 processes for average waiting, average turnaround, average response
    void calculateAverageWaitingTime();
    void calculateAverageTurnAroundTime();
    void calculateAverageResponseTime();

    double getAverageWaitingTime() const;
    double getAverageTurnAroundTime() const;
    double getAverageResponseTime() const;

    void setCurrentProcess(process process);
    process getCurrentProcess() const;
    vector <process>& getCompletedProcesses();

    virtual void addProcess(process p, int time);
    virtual void beginOperation(int time);
};




#endif //SCHEDULER_H
