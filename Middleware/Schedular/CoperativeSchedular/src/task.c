

#include "ModeManagement.h"
#include "lcd.h"
void task1(void) {
    if(ModeManagement.Mode == MODE_CONFIG)
    {
        ModeM_ConfigMode();
    }
}

void task2(void) {
    // Read sensor or toggle GPIO
    if(ModeManagement.Mode == MODE_NORMAL)
    {
    	EventMngt_Main();
    	lcd_process_jobs();
    }
}

void task3(void) {
    // Read sensor or toggle GPIO
}

