#ifndef MODEMANAGEMENT_H
#define MODEMANAGEMENT_H
#include <stdint.h>
#include <stdbool.h>

#define MODE_IDLE           0u
#define MODE_CONFIG_INIT    1u
#define MODE_CONFIG         2u
#define MODE_NORMAL         3u
#define MODE_SHUTDOWN       4u

#define CONFIG_STATE_UNINIT              0u
#define CONFIG_STATE_DISPLAY_VOTES       1u
#define CONFIG_STATE_NORMAL_MODE         2u
#define CONFIG_STATE_SHUTDOWN            3u
#define CONFIG_STATE_UART_CONNECT        4u
#define CONFIG_STATE_RESET_DATA          5u
#define CONFIG_STATE_MAIN_MENU           6u
#define CONFIG_STATE_VIA_PIN             7u


typedef struct
{
    uint8_t Mode;
    uint8_t ConfigSate;
}ModeManagement_St;

typedef enum{
	DCM_UNINIT,
	DCM_IDLE,
	DCM_WAITING,
	DCM_PROCESS_RX,
	DCM_PROCESS_TX,
}DCM_State_en;

typedef enum{
    DCM_NO_SERVICE,
    DCM_SESSION_CONTROL = 0x10,
    DCM_READ_RDID = 0x22,
    DCM_SECURITY_ACCESS = 0x27,
    CM_WRITE_DID = 0x2E,
    DCM_ROUTINE_CONTROL = 0x31,
    DCM_TESTER_PRESENT = 0x3E
}Dcm_service_en;
typedef struct{
	DCM_State_en state;
	uint8_t rxBuffer[1024u];
	uint8_t txBuffer[1024u];
	bool rxReady;
	bool txReady;
	uint8_t service;
	Dcm_service_en Dcm_service;
}Dcm_Process_st;

#define REQ_RX_IDLE 0x00u
#define REQ_RX_PCI 0x01u

extern ModeManagement_St ModeManagement;    
extern void ModeM_init();
extern void ModeM_ConfigMode_Ev_Callback(uint8_t pinIndex);
extern void ModeM_ConfigMode();
extern void EventMngt_Main();
extern void Dcm_Main(void);
#endif // MODEMANAGEMENT_H
