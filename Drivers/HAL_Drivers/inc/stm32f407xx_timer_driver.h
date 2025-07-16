#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
void timer_init(void);
extern void timer2_init(void);
extern void timer2_delay_ms(uint32_t ms);
extern void TIM2_IRQHandler(void);
extern uint8_t is_timer2_delay_elapsed();


void timer3_os_tick_init(void);
uint32_t os_timer_get_tick(void);
void timer2_init(void);



#endif // TIMER_H
