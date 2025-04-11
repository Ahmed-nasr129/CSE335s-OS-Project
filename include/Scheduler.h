#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <string>
#include <vector>
using namespace std;

class process {
private:
    int pid;
    int arrivalTime;
    int priority; // -1 in case of no priority
    int burstTime;
    int startTime;
    int completionTime;
    int remainingTime;
    int turnAroundTime;

public:
    process() : pid(0), arrivalTime(0), priority(-1), burstTime(0),
                startTime(0), completionTime(0), remainingTime(0),
                turnAroundTime(0) {}
    
    process(int pid, int arrival, int burst, int priority = -1) :
        pid(pid), arrivalTime(arrival), burstTime(burst), priority(priority),
        remainingTime(burst), startTime(0), completionTime(0), turnAroundTime(0) {}

    // Declaration only (no implementation)
    void setRemainingTime(int remaining);
    void setCompletionTime(int completion);
    void setArrivalTime(int arrival);
    void setStartTime(int start);

    int getRemainingTime() const;
    int getCompletionTime() const;
    int getBurstTime() const;
    int getPid() const;
    int getArrivalTime() const;
    int getStartTime() const;
    int getPriority() const;

    friend class Scheduler;
};

class Scheduler {
protected:
    process currentProcess;
    vector<process> completedProcesses;
    double averageWaitingTime;
    double averageTurnAroundTime;
    double averageResponseTime;

public:
    Scheduler();
    virtual ~Scheduler();

    // Declarations only (no implementation)
    void calculateAverageWaitingTime();
    void calculateAverageTurnAroundTime();
    void calculateAverageResponseTime();

    double getAverageWaitingTime() const;
    double getAverageTurnAroundTime() const;
    double getAverageResponseTime() const;

    void setCurrentProcess(process p);
    process getCurrentProcess() const;
    vector<process>& getCompletedProcesses();

    virtual void addProcess(process p, int time) = 0;
    virtual void beginOperation(int time) = 0;
};

#endif // SCHEDULER_H