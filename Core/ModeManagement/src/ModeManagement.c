#ifndef MODEMANAGEMENT_C
#define MODEMANAGEMENT_C
#include "ModeManagement.h"
#include "stm32f407xx_gpio_driver.h"
#include "EventManagement.h"
#include "lcd.h"
#include <string.h>

ModeManagement_St ModeManagement = {
    .Mode = MODE_IDLE
};

void ModeM_init()
{
    //start config Mode if config pin is pressed
    if((uint8_t)0x1 == (uint8_t)EventMngt_ReadButton(Event_GPIO_Handle[SYSCONFIGPIN_INDEX].GPIO_Handle_Ev.pGPIOx,
    									Event_GPIO_Handle[SYSCONFIGPIN_INDEX].GPIO_Handle_Ev.GPIO_PinConfig.GPIO_PinNumber))
    {
        ModeManagement.Mode       = MODE_CONFIG_INIT;
    } 
    else
    {
         ModeManagement.Mode = MODE_NORMAL;
         LCD_Job_t msg = {
             .type = LCD_JOB_STATIC,
             .duration_ms = 3000,
         };
         strcpy(msg.message, "Normal mode Entered");
         lcd_enqueue_job(msg);
         lcd_display_clear();
     	lcd_print_auto_wrap("Welcome Prashant");
    }   
    ModeManagement.ConfigSate = MODE_IDLE;
}

void ModeM_ConfigMode_Ev_Callback()
{
    ModeManagement.Mode = MODE_CONFIG;
    /* LCD Message: 
    * Config Mode
    * Press 1 to Display Votes
    * Press 2 to Normal Mode
    * Press 3 to Shutdown
    * Press 4 to Connect with UART
    * Press 5 to Reset All Data
    */
}
void ModeM_ConfigMode_State_Ev_Callback(uint8_t ConsigSate)
{
    if(ConsigSate > CONFIG_STATE_RESET_DATA)
    {
        /* LCD Message: Input Error */
    }
    else
    {
        ModeManagement.ConfigSate = CONFIG_STATE_DISPLAY_VOTES;
    }
  
}
void ModeM_ConfigMode()
{
    switch(ModeManagement.ConfigSate)
    {
        case CONFIG_STATE_DISPLAY_VOTES:
        {
            ModeManagement.ConfigSate = CONFIG_STATE_DISPLAY_VOTES;
            /* LCD Message: Display All candidates Votes 
            *  LCD Message: Press 6 for Main Menu
            */

            break;
        }
        case CONFIG_STATE_NORMAL_MODE:
        {
            /* LCD Message: Normal Mode */

            break;
        }
        case CONFIG_STATE_SHUTDOWN:
        {
            /* LCD Message: Shutdown Mode */
            /* Shutdown code */

            break;
        }
        case CONFIG_STATE_UART_CONNECT:
        {
            ModeManagement.ConfigSate = CONFIG_STATE_UART_CONNECT;
            /* LCD Message: UART Connect Mode */
            /* Connect with UART code*/

            break;
        }
        case CONFIG_STATE_RESET_DATA:
        {
            ModeManagement.ConfigSate = CONFIG_STATE_RESET_DATA;
            /* LCD Message: Reset Data Mode 
            *  LCD Message: Press 6 for Main Menu
            */
            /* Reset All permanently stored data */

            break;
        }
        default:
        {

        }
    }
}
#endif // MODEMANAGEMENT_C
