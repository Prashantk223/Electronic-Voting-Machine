#ifndef MODEMANAGEMENT_C
#define MODEMANAGEMENT_C
#include "ModeManagement.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_usart_driver.h"
#include "EventManagement.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

Dcm_Process_st Dcm_Process = {0};
ModeManagement_St ModeManagement = {
    .Mode = MODE_IDLE
};
const uint32_t EVM_IDENTIFICATION_NUMBER = 0xA0A0A0A0;

char ascii_array[10];

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
    }   
    ModeManagement.ConfigSate = MODE_IDLE;
}
static void ModeM_ConfigModeM_Message(void)
{
	//clear lcd queue
	lcd_job_clear();

    ModeManagement.Mode = MODE_CONFIG;
    /* LCD Message:
    * Config Mode Entered */
    LCD_Job_t msg = {
        .type = LCD_JOB_STATIC,
        .duration_ms = 1000,
    };
    strcpy(msg.message, "Config Mode Entered ");
    lcd_enqueue_job(msg);


    /* Press 1 to Display Votes */
        msg.type = LCD_JOB_STATIC;
        msg.duration_ms = 1000;

    strcpy(msg.message, "Press 1 to Display Votes  ");
    lcd_enqueue_job(msg);

    /* Press 2 to Normal Mode */
    msg.type = LCD_JOB_STATIC;
    msg.duration_ms = 1000;

    strcpy(msg.message, "Press 2 to Normal Mode ");
    lcd_enqueue_job(msg);

    /* Press 3 to Shutdown */
    msg.type = LCD_JOB_STATIC;
    msg.duration_ms = 1000;

    strcpy(msg.message, "Press 3 to Shutdown ");
    lcd_enqueue_job(msg);

    /* Press 4 to Reset All Data */
    msg.type = LCD_JOB_STATIC;
    msg.duration_ms = 1000;

    strcpy(msg.message, "Press 4 to Reset All Data  ");
    lcd_enqueue_job(msg);

    /* Press 5 to Connect with UART */
    msg.type = LCD_JOB_STATIC;
    msg.duration_ms = 1000;

    strcpy(msg.message, "Press 5 to Connect with UART ");
    lcd_enqueue_job(msg);


}

void ModeM_ConfigMode_Ev_Callback(uint8_t pinIndex)
{
    if(pinIndex == SYSCONFIGPIN_INDEX)
    {
    	ModeManagement.ConfigSate = CONFIG_STATE_VIA_PIN;
    	ModeM_ConfigModeM_Message();
    }
    else if(pinIndex == 0u)
    {
        lcd_job_clear();
        ModeManagement.ConfigSate = CONFIG_STATE_DISPLAY_VOTES;
    }
    else if(pinIndex == 1u)
    {
        lcd_job_clear();
        ModeManagement.ConfigSate = CONFIG_STATE_NORMAL_MODE;
    }
    else if(pinIndex == 2u)
    {
        lcd_job_clear();
        ModeManagement.ConfigSate = CONFIG_STATE_SHUTDOWN;
    }
    else if(pinIndex == 3u)
    {
        lcd_job_clear();
        ModeManagement.ConfigSate = CONFIG_STATE_RESET_DATA;
    }
    else if(pinIndex == 4u)
    {
        lcd_job_clear();
        ModeManagement.ConfigSate = CONFIG_STATE_UART_CONNECT;
    }
}

void ModeM_ConfigMode()
{


    switch(ModeManagement.ConfigSate)
    {
		case CONFIG_STATE_VIA_PIN:
			if(lcd_queue_size() == 0)
			{
				ModeM_ConfigModeM_Message();
			}
		break;
        case CONFIG_STATE_DISPLAY_VOTES:
        {
            //ModeManagement.ConfigSate = CONFIG_STATE_DISPLAY_VOTES;
            /* LCD Message: Display All candidates Votes 
            *  LCD Message: Press 6 for Main Menu
            */
            for(int i =0; i < NUM_CANDIDATES; i++)
            {
                LCD_Job_t msg = {
                    .type = LCD_JOB_STATIC,
                    .duration_ms = 1000,
                };
                strcpy(msg.message, CandidateVoting_Status[i].CandName);
                strcat(msg.message, " - ");
                sprintf(ascii_array, "%lu", CandidateVoting_Status[i].votes);
                strcat(msg.message, ascii_array);
                strcat(msg.message, " votes");
                lcd_enqueue_job(msg);
            }
            break;
        }
        case CONFIG_STATE_NORMAL_MODE:
        {
            /* LCD Message: Normal Mode */
            ModeManagement.ConfigSate = CONFIG_STATE_SHUTDOWN;
            break;
        }
        case CONFIG_STATE_SHUTDOWN:
        {
            /* LCD Message: Shutdown Mode */
            LCD_Job_t msg = {
            .type = LCD_JOB_STATIC,
            .duration_ms = 1000,
            };
            strcpy(msg.message, "Shutting down ");
            lcd_enqueue_job(msg);
            /* Shutdown code */
            ModeManagement.ConfigSate = CONFIG_STATE_UNINIT;
            break;
        }
        case CONFIG_STATE_UART_CONNECT:
        {
            /* LCD Message: UART Connect Mode */
            LCD_Job_t msg = {
            .type = LCD_JOB_STATIC,
            .duration_ms = 1000,
            };
            strcpy(msg.message, "UART Connect Mode");
            lcd_enqueue_job(msg);
            Dcm_Process.state = DCM_IDLE;

            ModeManagement.ConfigSate = CONFIG_STATE_UNINIT;
            break;
        }
        case CONFIG_STATE_RESET_DATA:
        {
            /* Reset All permanently stored data */
            LCD_Job_t msg = {
            .type = LCD_JOB_STATIC,
            .duration_ms = 1000,
            };
            strcpy(msg.message, "Erase All data");
            lcd_enqueue_job(msg);

            ModeManagement.ConfigSate = CONFIG_STATE_UNINIT;
            break;
        }
        default:
        {

        }
    }
}

void Dcm_Main(void)
{
    uint16_t RDID = 0;
    uint16_t routine = 0;
    char tempbuffer[1024];
    uint16_t ReqLength = (Dcm_Process.rxBuffer[0] << 8) | Dcm_Process.rxBuffer[1];
    switch(Dcm_Process.state)
    {
        case DCM_IDLE:
    		//First lets enable the reception in interrupt mode
    		//this code enables the receive interrupt
    		USART_ReceiveDataIT_Request(Dcm_Process.rxBuffer, 2u);
    		Dcm_Process.state = DCM_WAITING;
        break;
        case DCM_WAITING:
        if(Dcm_Process.rxReady == true)
        {
            Dcm_Process.state = DCM_PROCESS_RX;
            Dcm_Process.service = (Dcm_service_en)Dcm_Process.rxBuffer[2];
        }
        break;
        
        case DCM_PROCESS_RX:
            switch(Dcm_Process.service)
            {
                case DCM_READ_RDID:
                if(ReqLength == 3)
                {
                    RDID = (Dcm_Process.rxBuffer[3] << 8) | Dcm_Process.rxBuffer[4];
                    if(RDID == 0xF190)
                    {
                        //EVM Identification  Number     EVM_IDENTIFICATION     
                       //	Active EVM state Session
                        Dcm_Process.txBuffer[0] = 0x00;
                        Dcm_Process.txBuffer[1] = 0x07;
                        Dcm_Process.txBuffer[2] = 0x62;
                        Dcm_Process.txBuffer[3] = 0xF1u;
                        Dcm_Process.txBuffer[4] = 0x90u;
                        Dcm_Process.txBuffer[5] = (uint8_t)((EVM_IDENTIFICATION_NUMBER  & 0xFF000000) >> 24) ;
                        Dcm_Process.txBuffer[6] = (uint8_t)((EVM_IDENTIFICATION_NUMBER  & 0x00FF0000) >> 16) ;
                        Dcm_Process.txBuffer[7] = (uint8_t)((EVM_IDENTIFICATION_NUMBER  & 0x0000FF00) >> 8)  ;
                        Dcm_Process.txBuffer[8] = (uint8_t)((EVM_IDENTIFICATION_NUMBER  & 0xFF)) ;
                    }
                    else if(RDID == 0xF111)
                    {
                        //	Active EVM state Session  
                        Dcm_Process.txBuffer[0] = 0x00 ;
                        Dcm_Process.txBuffer[1] = 0x05 ;
                        Dcm_Process.txBuffer[2] = 0x62 ;
                        Dcm_Process.txBuffer[3] = 0xF1u ;
                        Dcm_Process.txBuffer[4] = 0x11 ;
                        Dcm_Process.txBuffer[5] = ModeManagement.Mode ;
                        Dcm_Process.txBuffer[6] = ModeManagement.ConfigSate ;
                    }
                    else
                    {
                        //NRC: Service Not Supported
                        Dcm_Process.txBuffer[0] = 0x00 ;
                        Dcm_Process.txBuffer[1] = 0x03 ;
                        Dcm_Process.txBuffer[2] = 0x7F ;
                        Dcm_Process.txBuffer[3] = 0x22 ;
                        Dcm_Process.txBuffer[4] = 0x11 ;
                    }
                }
                break;
                case CM_WRITE_DID:
                    RDID = (Dcm_Process.rxBuffer[3] << 8) | Dcm_Process.rxBuffer[4];
                    //00 05 2E F1 90 03
                    if((RDID == 0xF111) && (Dcm_Process.rxBuffer[5] == MODE_NORMAL))
                    {
                        //EVM Identification  Number     EVM_IDENTIFICATION     
                        //	Active EVM state Session  
                        Dcm_Process.txBuffer[0] = 0x00 ;
                        Dcm_Process.txBuffer[1] = 0x03 ;
                        Dcm_Process.txBuffer[2] = 0x6E ;
                        Dcm_Process.txBuffer[3] = 0xF1u ;
                        Dcm_Process.txBuffer[4] = 0x11 ;
                        
                        ModeManagement.Mode = MODE_NORMAL;
                        ModeManagement.ConfigSate = CONFIG_STATE_UNINIT;
                    }
                    else if((RDID == 0xF111) && (Dcm_Process.rxBuffer[5] == MODE_SHUTDOWN))
                    {
                        Dcm_Process.txBuffer[0] = 0x00 ;
                        Dcm_Process.txBuffer[1] = 0x03 ;
                        Dcm_Process.txBuffer[2] = 0x6E;
                        Dcm_Process.txBuffer[3] = 0xF1;
                        Dcm_Process.txBuffer[4] = 0x11;
                        
                        ModeManagement.Mode = MODE_SHUTDOWN;
                        ModeManagement.ConfigSate = CONFIG_STATE_UNINIT;
                    }
                    else
                    {
                        //NRC: Service Not Supported
                        Dcm_Process.txBuffer[0] = 0x00;
                        Dcm_Process.txBuffer[1] = 0x03;
                        Dcm_Process.txBuffer[2] = 0x7F;
                        Dcm_Process.txBuffer[3] = 0x22;
                        Dcm_Process.txBuffer[4] = 0x11;
                    }
                break;
                case DCM_ROUTINE_CONTROL:
                    uint32_t index = 0;
                    uint32_t Cand_index = 0;
                    // 00 05 31 01 FF 00
                    routine = (Dcm_Process.rxBuffer[4] << 8) | Dcm_Process.rxBuffer[5] ;
                    Dcm_Process.txBuffer[0] = 0x00;
                    Dcm_Process.txBuffer[1] = 0x04;
                    Dcm_Process.txBuffer[2] = 0x71;
                    Dcm_Process.txBuffer[3] = 0x01;
                    Dcm_Process.txBuffer[4] = Dcm_Process.rxBuffer[4];
                    Dcm_Process.txBuffer[5] = Dcm_Process.rxBuffer[5];
                     if(routine == 0xFF00)
                     {
                        //erase all data;
                        memset(CandidateVoting_Status, 0, sizeof(CandidateVoting_Status));
                        //set default values
                        memcpy(CandidateVoting_Status, CandidateVoting_Status_Default, sizeof(CandidateVoting_Status_t));
                     }
                     else if (0xFF01 == routine)
                     {
                        //00 05 31 01 FF 00 AA
                        if( Dcm_Process.rxBuffer[6] == 0xAA)
                        {
                            //read votes of all candidates. Response structure - nameLength(1byte) + Name(100bytes) + votes(4bytes)
                            index = 6;
                            for(int i = 0; i < NUM_CANDIDATES; i++)
                            {
                                Dcm_Process.txBuffer[index] = strlen(CandidateVoting_Status[i].CandName);
                                index++;
                                memcpy(&Dcm_Process.txBuffer[index], CandidateVoting_Status[i].CandName, strlen(CandidateVoting_Status[i].CandName));
                                index += strlen(CandidateVoting_Status[i].CandName);
                                //convert number to ascii
                                sprintf(ascii_array, "%lu", CandidateVoting_Status[i].votes);
                                memcpy(&Dcm_Process.txBuffer[index], ascii_array, strlen(ascii_array));
                                index += strlen(ascii_array);
                            }
                            Dcm_Process.txBuffer[1] += index;
                        }
                        else if((Dcm_Process.rxBuffer[5] <= NUM_CANDIDATES) && ( Dcm_Process.rxBuffer[5] > 0))
                        {
                            // read vote of candidate only 1 candidate
                            index = 6;
                            uint32_t Cand_index = Dcm_Process.rxBuffer[5] ;
                            Dcm_Process.txBuffer[index] = strlen(CandidateVoting_Status[Cand_index].CandName);
                            index++;
                            memcpy(&Dcm_Process.txBuffer[index], CandidateVoting_Status[Cand_index].CandName, strlen(CandidateVoting_Status[Cand_index].CandName));
                            index += strlen(CandidateVoting_Status[Cand_index].CandName);
                            //convert number to ascii
                            sprintf(ascii_array, "%lu", CandidateVoting_Status[Cand_index].votes);
                            memcpy(&Dcm_Process.txBuffer[index], ascii_array, strlen(ascii_array));
                            index += strlen(ascii_array);
                            Dcm_Process.txBuffer[1] += index;
                        }
                     }
                     else if (0xFF02 == routine)
                     {
                         /* display votes on lcd for all candidates */
                        //00 05 31 01 FF 00 AA
                        if( Dcm_Process.rxBuffer[5] == 0xAA)
                        {
                            index = 0;
                            for(int i = 0; i < NUM_CANDIDATES; i++)
                            {
                                tempbuffer[index] = strlen(CandidateVoting_Status[i].CandName);
                                index++;
                                memcpy(&tempbuffer[index], CandidateVoting_Status[i].CandName, strlen(CandidateVoting_Status[i].CandName));
                                index += strlen(CandidateVoting_Status[i].CandName);

                                
                                memcpy(&tempbuffer[index], " : ", 3);
                                index += 3;

                                //convert number to ascii
                                sprintf(ascii_array, "%lu", CandidateVoting_Status[i].votes);
                                memcpy(&tempbuffer[index], ascii_array, strlen(ascii_array));
                                index += strlen(ascii_array);
                                
                                lcd_job_clear();
                                LCD_Job_t msg = {
                                    .type = LCD_JOB_SCROLLING,
                                    .duration_ms = 300,
                                };
                                strcpy(msg.message, tempbuffer);
                                lcd_enqueue_job(msg);
                                lcd_display_clear();
                            }
                        }
                        else if((Dcm_Process.rxBuffer[5] <= NUM_CANDIDATES) && ( Dcm_Process.rxBuffer[5] > 0))
                        {
                            index = 0;
                            Cand_index =  Dcm_Process.rxBuffer[5] ;
                            tempbuffer[index] = strlen(CandidateVoting_Status[Cand_index].CandName);
                            index++;
                            memcpy(&tempbuffer[index], CandidateVoting_Status[Cand_index].CandName, strlen(CandidateVoting_Status[Cand_index].CandName));
                            index += strlen(CandidateVoting_Status[Cand_index].CandName);
                            //convert number to ascii
                            sprintf(ascii_array, "%lu", CandidateVoting_Status[Cand_index].votes);
                            memcpy(&tempbuffer[index], ascii_array, strlen(ascii_array));
                            index += strlen(ascii_array);

                            lcd_job_clear();
                            //queue the request
                            LCD_Job_t msg = {
                            .type = LCD_JOB_SCROLLING,
                            .duration_ms = 300,
                            };
                            strcpy(msg.message, tempbuffer);
                            lcd_enqueue_job(msg);
                            lcd_display_clear();
                        }
                     }
                     
                    else
                    {
                        //NRC: Service Not Supported
                        Dcm_Process.txBuffer[0] = 0x00;
                        Dcm_Process.txBuffer[1] = 0x03;
                        Dcm_Process.txBuffer[2] = 0x7F;
                        Dcm_Process.txBuffer[3] = 0x31;
                        Dcm_Process.txBuffer[4] = 0x11;
                    }
                break;
                case DCM_TESTER_PRESENT:
                    //send positive response
                    Dcm_Process.txBuffer[0] = 0x00;
                    Dcm_Process.txBuffer[1] = 0x02;
                    Dcm_Process.txBuffer[2] = 0x7E;
                    Dcm_Process.txBuffer[3] = 0x00;
                break;
                default:
                break;
            }
        Dcm_Process.state = DCM_PROCESS_TX;
        break;
        case DCM_PROCESS_TX:
			Dcm_Process.txReady = true; // handle transmission in UART_Main()
			Dcm_Process.rxReady = false; // reset rxReady for next operation
			//Send the msg indexed by cnt in blocking mode3
			USART_SendDataIT_Response(Dcm_Process.txBuffer, ((Dcm_Process.txBuffer[0] << 8) | Dcm_Process.txBuffer[1]));
			Dcm_Process.state = DCM_IDLE;
  	   break;

        
        default:
        break;
    }
}

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t event)
{
	static uint8_t request_rx_state = REQ_RX_IDLE;
   if(event == USART_EVENT_RX_CMPLT)
   {
	    if(request_rx_state == REQ_RX_IDLE)
	    {
	    	request_rx_state = REQ_RX_PCI;
	    	//Dcm_Process.rxBuffer[2] = '\0';
	    	//uint16_t len = asciiToInt(Dcm_Process.rxBuffer);
	    	uint16_t len = ((Dcm_Process.rxBuffer[1] << 8) + Dcm_Process.rxBuffer[1]);
	    	USART_ReceiveDataIT_Request(&Dcm_Process.rxBuffer[2], len);
	    }
	    else if((request_rx_state == REQ_RX_PCI) && (Dcm_Process.state == DCM_WAITING))
		{
	 	    Dcm_Process.rxReady = true;
	 	   request_rx_state = REQ_RX_IDLE;
		}
	    else
	    {
	    	//ignore data
	    }

   }else if (event == USART_EVENT_TX_CMPLT)
   {
	   Dcm_Process.txReady = false;
	   Dcm_Process.rxReady = false;
	   Dcm_Process.state = DCM_IDLE;
   }
}

#endif // MODEMANAGEMENT_C
