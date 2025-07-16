#ifndef TASK_H_
#define TASK_H_

extern void task1(void);
extern void task2(void);
extern void task3(void);

typedef struct {
    void (*task_func)(void);  // Task function pointer
    uint32_t interval_ms;     // How often to run
    uint32_t last_run_ms;     // Last run time
} Task;

#endif // TASK_H
