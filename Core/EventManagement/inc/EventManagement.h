#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H
#include <stdint.h>
#include "stm32f407xx_gpio_driver.h"

typedef struct{
	char CandName[100];
	const GPIO_RegDef_t *pGPIOx;
    const uint8_t PinNumber;
    uint8_t PinStatus;
    uint32_t votes;
} CandidateVoting_Status_t;

typedef struct
{
	GPIO_Handle_t GPIO_Handle_Ev;       		/*!< This holds the base address of the GPIO port to which the pin belongs >*/
	uint8_t IRQ_Num;   /*!< This holds GPIO pin configuration settings >*/
}Event_GPIO_Handle_t;

typedef enum
{
	Event_Key_Idle,
	Event_Key_Pressed,
	Event_Key_Debounced,
	Event_Key_Confirm
}Event_Key_Status_t;

typedef struct
{
	Event_Key_Status_t Status;
	uint8_t Key_Index;
}Event_Key_t;

#define DUMMY_PIN 								0u

#define NUM_CANDIDATES 5u

#define NUM_EVENT_PINS 6u
#define PIN_DUMMY_ALT_FUNCTION 0u
#define SYSCONFIGPIN_INDEX 5u

//extern variables
extern CandidateVoting_Status_t CandidateVoting_Status[NUM_CANDIDATES];
extern CandidateVoting_Status_t CandidateVoting_Status_Default[NUM_CANDIDATES] ;
extern Event_GPIO_Handle_t Event_GPIO_Handle[NUM_EVENT_PINS];

//extern functions
extern uint8_t EventMngt_ReadButton(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
extern void EventMngt_Init();

#endif // EVENTMANAGEMENT_H
