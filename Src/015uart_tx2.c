/*
 * uart_tx.c
 *
 *  Created on: Jan 22, 2019
 *      Author: admin
 */

#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"
#include "EventManagement.h"
#include "ModeManagement.h"
#include "lcd.h"
#include "stm32f407xx_timer_driver.h"
#include "sched.h"

//we have 3 different messages that we transmit to arduino
//you can by all means add more messages
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024] ;

extern USART_Handle_t usart2_handle;


//This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

extern void initialise_monitor_handles();




void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn,GpioLed;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD,ENABLE);

	GPIO_Init(&GpioLed);

}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}
int main(void)
{
	uint32_t cnt = 0;


	EventMngt_Init();
	ModeM_init();
	GPIO_ButtonInit();
	USART_Init_DCM();

	lcd_init();
	lcd_print_auto_wrap("Welcome... ");

	timer_init();
	//Timer init
	__enable_irq();;

	scheduler_loop();

    printf("Application is running\n");





    //do forever
    while(1)
    {


    }
	return 0;
}

void testingfun(void)
{
	uint32_t cnt = 0;

	//wait till button is pressed

	//to avoid button de-bouncing related issues 200ms of delay
	delay();

	// Next message index ; make sure that cnt value doesn't cross 2
	cnt = cnt % 3;

	//First lets enable the reception in interrupt mode
	//this code enables the receive interrupt
	//while ( USART_ReceiveDataIT(&usart2_handle,rx_buf,strlen(msg[cnt])) != USART_READY );

	//Send the msg indexed by cnt in blocking mode
	//USART_SendData(&usart2_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));

	USART_SendDataIT_Response((uint8_t*)msg[cnt],strlen(msg[cnt]));

	printf("Transmitted : %s\n",msg[cnt]);


	//Now lets wait until all the bytes are received from the arduino .
	//When all the bytes are received rxCmplt will be SET in application callback
	//while(rxCmplt != SET);

	//just make sure that last byte should be null otherwise %s fails while printing
	rx_buf[strlen(msg[cnt])+ 1] = '\0';

	//Print what we received from the arduino
	printf("Received    : %s\n",rx_buf);

	//invalidate the flag
	rxCmplt = RESET;

	//move on to next message indexed in msg[]
	cnt ++;
}
