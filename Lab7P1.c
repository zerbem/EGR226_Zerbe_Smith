#include "msp.h"

#define RS 1         //4.0 mask
#define EN 4         //4.2 mask

void systick_init();
void systick_delay(uint16_t millsec);
void nibble_write(unsigned char data, unsigned char control);
void commandWrite(unsigned char command);
void dataWrite(unsigned char data);
void LCD_init();
void port4_init();


void main(void)
{
    port4_init();
    systick_init();
    LCD_init();
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	while(1){
	    commandWrite(1);
	    systick_delay(500);
	}
}

void LCD_init()
{
    P4->DIR = 0xFF;
    systick_delay(30);
    nibble_write(0x30, 0);
    systick_delay(10);
    nibble_write(0x30, 0);
    systick_delay(1);
    nibble_write(0x30, 0);
    systick_delay(1);
    nibble_write(0x20, 0);
    systick_delay(1);

    commandWrite(0x28);
    commandWrite(0x06);
    commandWrite(0x01);
    commandWrite(0x0F);
}
void nibble_write(unsigned char data, unsigned char control)
{
    data &= 0xF0;
    control &=0x0F;
    P4->OUT = data | control;
    P4->OUT = data | control | EN;
    systick_delay(1);
    P4->OUT = data;
    P4->OUT = 0x00;
}
void commandWrite(unsigned char command)
{
    nibble_write(command & 0xF0, 0);
    nibble_write(command << 4, 0);
    systick_delay(4);
}
void dataWrite(unsigned char data)
{
    nibble_write(data & 0xF0, RS);
    nibble_write(data << 4, RS);
    systick_delay(1);
}
void systick_init()
{
    /*****************************************
     * Brief:   initializes the systick timer.
     * Params:
     *          VOID
     * Returns:
     *          VOID
     *****************************************/
    SysTick->CTRL = 0x00;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL  = 0x00;
    SysTick->CTRL = 0x00000005;
}
void port4_init()
{
    /*******************************
     * Brief:       Initializes all of P4 to inputs with 4.0,1,2,3 REN pull up
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    P4->SEL0 &= 0x00;           //Clearing all of P4 SEL
    P4->SEL1 &= 0x00;
    P4->DIR  |= 0xFF;           //Set all to outputs
    P4->OUT  &= 0x00;           //Clear all output values
}
void systick_delay(uint16_t delaymS)
{
    /*******************************************************
     * Brief:   A delay function that uses the systick timer
     *              clocked with the 3 MHz master clock.
     * Params:
     *          uint16_t delaymS:   amount of milliseconds to
     *                                  delay for.
     * Returns:
     *          VOID
     **********************************************************/
    SysTick->LOAD   = ((delaymS * 3000)-1);
    SysTick->VAL    = 0;
    while((SysTick->CTRL & 0x00010000)==0);
}
