#ifndef MODEMANAGEMENT_H
#define MODEMANAGEMENT_H

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
#include <stdint.h>
typedef struct
{
    uint8_t Mode;
    uint8_t ConfigSate;
}ModeManagement_St;

extern ModeManagement_St ModeManagement;    
extern void ModeM_init();
extern void ModeM_ConfigMode_Ev_Callback();
extern void ModeM_ConfigMode();
extern void EventMngt_Main();

#endif // MODEMANAGEMENT_H
