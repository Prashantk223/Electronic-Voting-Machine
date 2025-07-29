
/*
Preemptive scheduler (with round-robin for equal priorities)
Delay and SysTick handling
Semaphores (priority-aware)
Mutexes with priority inheritance (declared, ready to expand)
Idle task fallback
Stack initialization and switching with PendSV
*/
#include <stdint.h>
#include <stddef.h>
#include "cmsis_gcc.h"
#include "stm32f4xx.h"          /* replace with your MCU header */

#define MAX_TASKS      5u
#define STACK_WORDS     128uu
#define OS_TICK_HZ      1000
typedef enum{
    TASK_IDLE,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED
}task_state_t;
typedef struct{

    uint32_t *stackptr;
    uint8_t base_priority;
    task_state_t state;
    uint32_t *taskptr;
    uint32_t delay;
}TCB_st;

static TCB_st TCB_st[MAX_TASKS];
static uint32_t stacks[MAX_TASKS][STACK_WORDS];
static volatile int current_task = 0;
static volatile int next_task = 0;
static volatile tick_count = 0;

//Function declarations
static void schedule(void);
static void idle_task(void)  __attribute__((noreturn));
static uint32_t* init_stack(uint32_t* top, void(*entry)(void));

static uint32_t* init_stack(uint32_t* top, void(*entry)(void))
{
  
    /*init_stack() builds that initial exception frame so the scheduler can simply load the Process Stack Pointer (PSP)
     with the returned address and perform a context‑switch—the very first BX LR the CPU executes will jump straight into the task’s entry function*/
    top -= 8;
    top[4] = 0;   // R12
    top[3] = 0;   // R3
    top[2] = 0;   // R2
    top[1] = 0;   // R1
    top[0] = 0;   // R0
    top[7] = 0x01000000U;             // xPSR (bit 24 = Thumb bit must be set)
    top[6] = (uint32_t)entry;         // PC (task entry function)
      /* Link Register (LR) is used to store the return address when a function or exception is called — it's a core part of function calls and task switching.
    When you call a function with BL func (Branch with Link):BL my_function
    The address of the next instruction is stored in LR. Inside the function, when you execute:
    In Cortex-M Exception Handling: When an exception occurs (e.g. SysTick, PendSV):
    The processor saves:
        R0–R3, R12, LR, PC, xPSR
    It does not save R4–R11. It stores an EXC_RETURN code in LR, not a return address.This tells the CPU: Return to Thread mode,  Use the Process Stack Pointer (PSP), No floating-point context
    */
    top[5] = 0xFFFFFFFDU;             // LR (EXC_RETURN: return to Thread mode using PSP, no FPU)

    return top;
}

int os_create_task(void(*fn)(void), uint8_t priority)
{
    static int idx = 0;
    if(idx >= MAX_TASKS) return -1;

    TCB_st[idx].base_priority = priority;
    TCB_st[idx].stackptr = init_stack(&stacks[idx][STACK_WORDS], fn);
    TCB_st[idx].state = TASK_READY;
    TCB_st[idx].taskptr = fn;

    return idx++;
}
static void schedule(void)
{
    int best_prio = 255; // lowest priority
    int candidates[MAX_TASKS];
    int count = 0;
    for(int i = 0; i < MAX_TASKS; i++)
    {
        if(TCB_st[i].state == TASK_READY)
        {
            if(best_prio > TCB_st[i].base_priority)
            {
                best_prio = TCB_st[i].base_priority;
                count = 0;
                candidates[count++] = i;
            }
            else if(best_prio == TCB_st[i].base_priority)
            {
                candidates[count++] = i;
            }
        }
    }

    if(count == 0)
    {
        next_task = 0; //idle task
    }
    else if(count == 1)
    {
        next_task = candidates[0];
    }
    else
    {
        bool found = false;
        // Round-robin for equal priority tasks
        for(int i = 0; i < MAX_TASKS && !found; i++)
        {
            uint8_t idx = (current_task + i + 1) % MAX_TASKS;
            for(int j = 0; j < count; j++)
            {
                if(candidates[j] == idx)
                {
                    next_task = candidates[j];
                    found = true;
                    break;
                }
            }
        }
    }

    if(next_task != current_task)
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

}

void PendSV_Handler(void) {
	tcb[next_task].state = TASK_RUNNING;

    __asm volatile (
        "MRS R0, PSP            \n"
        "STMDB R0!, {R4-R11}    \n"
        "LDR R1, =current_task  \n"
        "LDR R1, [R1]           \n"
        "CMP R1, #0             \n"
        "BLT skip_store         \n"
        "LDR R2, =tcb           \n"
        "LSLS R1, R1, #5        \n"
        "ADD R2, R2, R1         \n"
        "STR R0, [R2]           \n"
        "skip_store:            \n"
        "LDR R1, =next_task     \n"
        "LDR R1, [R1]           \n"
        "LDR R2, =current_task  \n"
        "STR R1, [R2]           \n"
        "LDR R2, =tcb           \n"
        "LSLS R1, R1, #5        \n"
        "ADD R2, R2, R1         \n"
        "LDR R0, [R2]           \n"
        "LDMIA R0!, {R4-R11}    \n"
        "MSR PSP, R0            \n"
        "BX LR                  \n"
    );

    //make current and next the same task
    current_task = next_task;
}

void os_delay(uint32_t ticks)
{
    __disable_irq();
    tcb[current_task].delay = ticks;
    tcb[current_task].state = TASK_BLOCKED;
    __enable_irq();

    schedule();
}

void SysTick_Handler(void)
{
        for(int i = 0; i < MAX_TASKS && !found; i++)
        {
            if(tcb[current_task].delay)
            {
                tcb[current_task].delay--;
                if(tcb[current_task].state == TASK_BLOCKED)
                {
                    tcb[current_task].state = TASK_READY
                }
            } 
        }
        
    //Keep the current task in ready pool
	tcb[current_task].state = TASK_READY;
    schedule();
}

void os_start(void)
{
    tcb[current_task].state = TASK_RUNNING;
    __set_PSP((uint32_t)tcb[current_task].stackptr);
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	/* Switch Thread mode to PSP*/
    __asm volatile (
        "MOV R0, #2         \n" //Prepare to use PSP (keep privileged)
        "MSR CONTROL, R0    \n" //Tell CPU to use PSP in Thread mode
        "ISB                \n" //Force the change to take effect immediately
    );
    while (1);
}