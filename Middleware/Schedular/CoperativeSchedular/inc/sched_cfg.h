#ifndef SCHED_CFG_H
#define SCHED_CFG_H

#define NUM_TASKS 3

#include "task.h"
#include<stdint.h>

Task task_table[NUM_TASKS] = {
    { task1, 10, 0 },   // Run every 10ms
    { task2, 50, 0 },   // Run every 50ms
    { task3, 100, 0 }   // Run every 100ms
};

#endif // SCHED_CFG_H
