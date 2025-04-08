#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <utility> 

using namespace std;

typedef struct {
    string name;
    int arrival;
    int priority; 
    int burst;
    int start = -1;
    int end = -1;
    int remaining;
    bool finished = false;
    vector<pair<int, int>> executionPeriods; 
} process;

#endif