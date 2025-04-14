//
// Created by Jana Aly on 4/9/2025.
//

    #include "RR_Scheduler.h"

    RR_Scheduler::RR_Scheduler(int qt) {
        setQT(qt);
    }

    void RR_Scheduler::setQT(int QT) {
        qt=QT;
    }

    void RR_Scheduler::addProcess(process p, int time) {
        p.setArrivalTime(time);
        readyQueue.push(p);
        if(cnt==0) {
            currentProcess=readyQueue.front();
            readyQueue.pop();
        }
        cnt++;
    }

    void RR_Scheduler::beginOperation(int time) {
        roundRobin(time);
    }

    void RR_Scheduler::roundRobin(int time) {
        if(!readyQueue.empty() || completedProcesses.size()<cnt) {
            updateQueue(time);
        }
    }

    void RR_Scheduler::updateQueue(int time) {
        if(currentProcess.getRemainingTime()==currentProcess.getBurstTime()) {
            currentProcess.setStartTime(time);
        }
        //remaining -=1  decreased ++
        int remainingTime=currentProcess.getRemainingTime();
        currentProcess.setRemainingTime(--remainingTime);
        decreased++;
        if(decreased==qt && currentProcess.getRemainingTime()>0) {
            decreased=0; //reset decreased
            readyQueue.push(currentProcess);
            currentProcess=readyQueue.front();
            readyQueue.pop();
        }

        else if(currentProcess.getRemainingTime()==0) {
            completedProcesses.push_back(currentProcess);
            currentProcess.setCompletionTime(time);
            if(!readyQueue.empty()) {
                decreased=0;     //reset decreased for new process to enter cpu
                currentProcess=readyQueue.front();
                readyQueue.pop();
            }
        }
    }





