

#include "ModeManagement.h"
#include "lcd.h"
void task1(void) {
    if(ModeManagement.Mode == MODE_CONFIG)
    {
        ModeM_ConfigMode();
        Dcm_Main();
    }
}

void task2(void) {
    // Read sensor or toggle GPIO
    if(ModeManagement.Mode == MODE_NORMAL)
    {
    }
	EventMngt_Main();
}
extern 	void testingfun(void);
void task3(void) {
	//testingfun();
    lcd_process_jobs();
}

