#include "stm32f407xx_timer_driver.h"
#include "stm32f407xx.h"

static volatile uint32_t delay_counter = 0;
static void TIM2_IRQHandler_Handler();
volatile uint32_t os_tick_count = 0;

void timer_init(void)
{
	timer2_init();
    timer3_os_tick_init();
}
void timer2_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->CR1 = 0;
    TIM2->PSC = 16000 - 1;       // 1 ms tick
    TIM2->ARR = 1;             // Count to 1000 → 1 second
    TIM2->CNT = 0;

    TIM2->EGR |= 1;              // Force update, apply ARR/PSC immediately

    TIM2->SR &= ~TIM_SR_UIF;     // Clear update flag
    TIM2->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
}


void timer2_delay_ms(uint32_t ms)
{
    delay_counter = ms;

    // Start the timer
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}



void TIM2_IRQHandler(void)
{
    TIM2_IRQHandler_Handler();
}
uint8_t delay_done;
void TIM2_IRQHandler_Handler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;

        if (delay_counter > 0)
        {
            delay_counter--;

            if (delay_counter == 0)
            {
                TIM2->CR1 &= ~TIM_CR1_CEN;
                delay_done = 1; // Signal that delay is over
            }
        }
    }
}

uint8_t is_timer2_delay_elapsed(void)
{
	return delay_done;
}






void timer3_os_tick_init(void)
{
    // 1. Enable TIM3 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 2. Disable timer before configuration
    TIM3->CR1 = 0;

    // 3. Set prescaler and ARR for 1ms tick (assuming 16MHz clock)
    TIM3->PSC = 16000 - 1;  // 1 tick = 1ms
    TIM3->ARR = 1;          // Overflow every 1 tick

    TIM3->CNT = 0;

    // 4. Generate update event to load PSC/ARR
    TIM3->EGR |= TIM_EGR_UG;

    // 5. Enable update interrupt
    TIM3->DIER |= TIM_DIER_UIE;

    // 6. Enable TIM3 interrupt in NVIC
    NVIC_EnableIRQ(TIM3_IRQn);

    // 7. Start the timer
    TIM3->CR1 |= TIM_CR1_CEN;
}

uint32_t os_timer_get_tick(void)
{
    return os_tick_count;
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)
    {
        TIM3->SR &= ~TIM_SR_UIF;  // Clear the interrupt flag
        os_tick_count++;          // Increment OS tick
    }
}

