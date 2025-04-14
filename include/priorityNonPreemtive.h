#ifndef PRIORITYNONPREEMTIVE_H
#define PRIORITYNONPREEMTIVE_H

#include "Scheduler.h"

class priorityNonPreemtive :public  Scheduler {
    private:
    typedef struct  {
        bool operator()(const process& a, const process& b) {
            if (a.getPriority() == b.getPriority())
                return a.getArrivalTime() > b.getArrivalTime();
            return a.getPriority() > b.getPriority();
        }
    }compare;

    priority_queue<process,vector<process>,compare> readyQeueue;

    public:
    priorityNonPreemtive(){}
    ~priorityNonPreemtive(){}
    void addProcess(process p, int time) override;
    void beginOperation(int time) override;

    priority_queue<process,vector<process>,compare> get_ready_qeueue() const ;
};

#endif //PRIORITY_H
