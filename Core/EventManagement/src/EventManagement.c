#ifndef EVENT_MANAGEMENT_C
#define EVENT_MANAGEMENT_C

#include "EventManagement.h"
#include "ModeManagement.h"
#include <stm32f407xx_gpio_driver.h>
#include <stm32f407xx_timer_driver.h>
#include <string.h>
#include "lcd.h"

Event_Key_t Event_Key;
static void EventMngt_ButtonEvent(void);
void EventMngt_ButtonDebounceIRQ_CONFIG_BUTTON(void);
void EventMngt_Init()
{
	Event_Key.Status = 	Event_Key_Idle;
    for(int i = 0; i < NUM_CANDIDATES; i++)
    {
        CandidateVoting_Status[i].PinStatus = 0;
        CandidateVoting_Status[i].votes = 0;

    }
    for(int i = 0; i < NUM_EVENT_PINS; i++)
    {
    	GPIO_PeriClockControl(Event_GPIO_Handle[i].GPIO_Handle_Ev.pGPIOx, ENABLE);
        GPIO_Init(&Event_GPIO_Handle[i].GPIO_Handle_Ev);
    	GPIO_IRQInterruptConfig(Event_GPIO_Handle[i].IRQ_Num, ENABLE);
    }
     //set default values
    memcpy(CandidateVoting_Status, CandidateVoting_Status_Default, sizeof(CandidateVoting_Status_Default));
}
uint8_t EventMngt_ReadButton(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    uint8_t ret = 0;
    if((uint8_t)0x1 == (uint8_t)GPIO_ReadFromInputPin(pGPIOx, PinNumber))
    {
        ret = 1;
        //start debounce timer
    }
    return ret; 
}
void EXTI0_IRQHandler(void)
{
  //start debounce timer
	timer2_delay_ms(100);
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	Event_Key.Status = 	Event_Key_Pressed;
	Event_Key.Key_Index = 	0;
}
void EXTI1_IRQHandler(void)
{
  //start debounce timer
	timer2_delay_ms(100);
	GPIO_IRQHandling(GPIO_PIN_NO_1);
	Event_Key.Status = 	Event_Key_Pressed;
	Event_Key.Key_Index = 	1;
}
void EXTI2_IRQHandler(void)
{
  //start debounce timer
	timer2_delay_ms(100);
	GPIO_IRQHandling(GPIO_PIN_NO_2);
	Event_Key.Status = 	Event_Key_Pressed;
	Event_Key.Key_Index = 	2;
}

void EXTI3_IRQHandler(void)
{
  //start debounce timer
	timer2_delay_ms(100);
	GPIO_IRQHandling(GPIO_PIN_NO_3);
	Event_Key.Status = 	Event_Key_Pressed;
	Event_Key.Key_Index = 	3;
}
void EXTI4_IRQHandler(void)
{
  //start debounce timer
	timer2_delay_ms(100);
	GPIO_IRQHandling(GPIO_PIN_NO_4);
	Event_Key.Status = 	Event_Key_Pressed;
	Event_Key.Key_Index = 	4;
}
void EXTI9_5_IRQHandler(void)
{
  //start debounce timer
	timer2_delay_ms(100);
	GPIO_IRQHandling(GPIO_PIN_NO_8);
	Event_Key.Status = 	Event_Key_Pressed;
	Event_Key.Key_Index = 	5;
}
void EventMngt_Main()
{
	if(Event_Key.Status == Event_Key_Pressed)
	{
		if (is_timer2_delay_elapsed() == 1)
		{
			  Event_Key.Status = 	Event_Key_Debounced;
			  if(ModeManagement.Mode == MODE_NORMAL)
			  {
					  EventMngt_ButtonEvent();
			  }
			  else if(ModeManagement.Mode == MODE_CONFIG)
			  {
				  //if config mode, call the config mode callback
				  ModeM_ConfigMode_Ev_Callback(Event_Key.Key_Index);
			  }
		}
	}

}
static void EventMngt_ButtonEvent(void)
{
    GPIO_RegDef_t *pGPIOx = Event_GPIO_Handle[Event_Key.Key_Index].GPIO_Handle_Ev.pGPIOx;
    if((uint8_t)0x1 == (uint8_t)GPIO_ReadFromInputPin(pGPIOx,
    		Event_GPIO_Handle[Event_Key.Key_Index].GPIO_Handle_Ev.GPIO_PinConfig.GPIO_PinNumber))
    {
        //update the button status
        for(int i =0; i < NUM_CANDIDATES; i++)
        {
            if(CandidateVoting_Status[i].PinNumber ==
            		Event_GPIO_Handle[Event_Key.Key_Index].GPIO_Handle_Ev.GPIO_PinConfig.GPIO_PinNumber)
            {
                CandidateVoting_Status[i].votes++;


                LCD_Job_t msg = {
                    .type = LCD_JOB_SCROLLING,
                    .duration_ms = 100,
                };
                strcpy(msg.message, "You voted for ");
                strcat(msg.message, CandidateVoting_Status[i].CandName);
                lcd_enqueue_job(msg);
                lcd_display_clear();
                break;
            }
            else if(Event_Key.Key_Index == SYSCONFIGPIN_INDEX)
            {
                //if config button pressed, call the config mode callback
                EventMngt_ButtonDebounceIRQ_CONFIG_BUTTON();
            }
            else
            {
                //if not a valid button, reset the event key status

            }
        }
    }

	Event_Key.Status = 	Event_Key_Idle;
}
void EventMngt_ButtonDebounceIRQ_CONFIG_BUTTON(void)
{
    //dummy
    GPIO_RegDef_t *pGPIOx = Event_GPIO_Handle[SYSCONFIGPIN_INDEX].GPIO_Handle_Ev.pGPIOx;
    if((uint8_t)0x1 == (uint8_t)GPIO_ReadFromInputPin(pGPIOx, Event_GPIO_Handle[SYSCONFIGPIN_INDEX].GPIO_Handle_Ev.GPIO_PinConfig.GPIO_PinNumber))
    {
        //callback to Mode management to set config mode
        ModeM_ConfigMode_Ev_Callback(5);
    }
}

#endif // EVENT_MANAGEMENT_C
