
#include "lcd.h"
#include "stm32f407xx_gpio_driver.h"

LCD_JobQueue_t LCD_JobQueue = {0};

static void write_4_bits(uint8_t value);
static void lcd_enable(void);
static void mdelay(uint32_t cnt);
static void udelay(uint32_t cnt);

void lcd_send_command(uint8_t cmd)
{
	/* RS=0 for LCD command */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/*R/nW = 0, for write */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(cmd >> 4);
	write_4_bits(cmd & 0x0F);

}

/*
 *This function sends a character to the LCD
 *Here we used 4 bit parallel data transmission.
 *First higher nibble of the data will be sent on to the data lines D4,D5,D6,D7
 *Then lower nibble of the data will be set on to the data lines D4,D5,D6,D7
 */
void lcd_print_char(uint8_t data)
{
	/* RS=1 for LCD user data */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);

	/*R/nW = 0, for write */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(data >> 4);  /*Higher nibble*/
	write_4_bits(data & 0x0F); /*Lower nibble*/

}
void lcd_print_auto_wrap(const char* str)
{
    // Print to first line
    lcd_set_cursor(0, 0);
	mdelay(100);
    int count = 0;

    while (*str && count < 16)
    {
        lcd_print_char(*str++);
        count++;
    }

	mdelay(100);
    // If more characters exist, print to second line
    if (*str != '\0')
    {
        lcd_set_cursor(1, 0);
    	mdelay(100);
        count = 0;
        while (*str && count < 16)
        {
            lcd_print_char(*str++);
            count++;
        }
    }
}


void lcd_print_string(char *message)
{

      do
      {
          lcd_print_char((uint8_t)*message++);
      }
      while (*message != '\0');

}


void lcd_init(void)
{

	//1. Configure the gpio pins which are used for lcd connections

	GPIO_Handle_t lcd_signal;

	lcd_signal.pGPIOx = LCD_GPIO_PORT;
	lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;
	lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;
	GPIO_Init(&lcd_signal);

	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

	//2. Do the LCD initialization

	mdelay(40);

	/*RS = 0 , For LCD command */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/* RnW = 0, Writing to LCD */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(0x3);

	mdelay(5);

	write_4_bits(0x3);

	udelay(150);

	write_4_bits(0x3);
	write_4_bits(0x2);

    // Now LCD is in 4-bit mode
    lcd_send_command(0x28); // 4-bit, 2 line, 5x8 font
	mdelay(100);
    lcd_send_command(0x0C); // Display ON, cursor OFF
	mdelay(100);
    lcd_send_command(0x06); // Entry mode: increment, no shift
	mdelay(100);
    lcd_send_command(0x01); // Clear display
    mdelay(100);              // Wait for clear to complete



}

/* writes 4 bits of data/command on to D4,D5,D6,D7 lines */
static void write_4_bits(uint8_t value)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D4, ((value >> 0) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D5, ((value >> 1) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D6, ((value >> 2) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D7, ((value >> 3) & 0x1) );

	lcd_enable();

}

void lcd_display_clear(void)
{
	//Display clear
	lcd_send_command(LCD_CMD_DIS_CLEAR);

	/*
	 * check page number 24 of datasheet.
	 * display clear command execution wait time is around 2ms
	 */

	mdelay(2);
}


/*Cursor returns to home position */
void lcd_display_return_home(void)
{

	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of datasheet.
	 * return home command execution wait time is around 2ms
	 */
	mdelay(2);
}


/**
  *   Set Lcd to a specified location given by row and column information
  *   Row Number (1 to 2)
  *   Column Number (1 to 16) Assuming a 2 X 16 characters display
  */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
    uint8_t address = (row == 0) ? (0x00 + column) : (0x40 + column);
    lcd_send_command(0x80 | address);
}




static void lcd_enable(void)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	udelay(10);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	udelay(100);/* execution time > 37 micro seconds */
}

uint8_t lcd_enqueue_job(LCD_Job_t job)
{
	if(LCD_JobQueue.count >= LCD_JOB_QUEUE_SIZE)
		return 0;

	LCD_JobQueue.jobs[LCD_JobQueue.tail] = job;
	LCD_JobQueue.count++;
	LCD_JobQueue.tail = (LCD_JobQueue.tail + 1) % LCD_JOB_QUEUE_SIZE;
	return 1;
}

void lcd_process_jobs(void)
{
    static LCD_Job_t *activeJob = NULL;
    static uint32_t lastTick = 0;
    static uint32_t os_time_tick = 0;
    static uint8_t scrollIndex = 0;

    if((LCD_JobQueue.count != 0) && (!activeJob))
	{
    	activeJob = &LCD_JobQueue.jobs[LCD_JobQueue.head];
    	LCD_JobQueue.head = (LCD_JobQueue.head + 1) % LCD_JOB_QUEUE_SIZE;
    	LCD_JobQueue.count--;
    	lastTick = os_time_tick;
	}

    switch(activeJob->type)
    {
    	case LCD_JOB_STATIC:
    		if((os_time_tick - lastTick) < activeJob->duration_ms)
    		{
        		lcd_print_auto_wrap(activeJob->message);
    		}
    		else
    		{
    			activeJob = 0;
    			lcd_display_clear();
    		}
    	break;

    	case LCD_JOB_SCROLLING:
    		if((os_time_tick - lastTick) < activeJob->duration_ms)
    		{
    			lcd_print_auto_wrap(&activeJob->message[scrollIndex]);
    		}
    		else
    		{
    			scrollIndex++;
    			if(scrollIndex < 16)
    			{
    				lastTick = 0;
    			}
    			else
    			{
        			activeJob = 0;
        			lcd_display_clear();
    			}
    		}
    	break;

    	case LCD_JOB_CONSTANT:
    		lcd_print_auto_wrap(activeJob->message);
			activeJob = 0;
    	break;
    	default:
    		break;
    }

}
static void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1000); i++);
}

static void udelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1); i++);
}
