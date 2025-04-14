#ifndef FCFS_H
#define FCFS_H
#include "Scheduler.h"
#pragma once
using namespace std;

class FCFS_Scheduler : public Scheduler{
private:
  queue<process> processQueue;

public:
  FCFS_Scheduler();

  void addProcess(process p,int time) override;
  void beginOperation(int time) override;

};

#endif //FCFS_H
