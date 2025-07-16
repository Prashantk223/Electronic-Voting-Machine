#include "sched.h"
#include "sched_cfg.h"
#include "stm32f407xx_timer_driver.h"

uint32_t get_millis();  // Function returning system uptime in ms

void scheduler_loop() {
    while(1)
    {
        uint32_t now = os_timer_get_tick();
        for(int i = 0; i < NUM_TASKS; i++)
        {
            if((now - task_table[i].last_run_ms) >=  task_table[i].interval_ms)
            {
                task_table[i].task_func();
                task_table[i].last_run_ms = now;
            }
        }
    }
}

