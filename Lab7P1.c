/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 7- LCD interfacing
 * File:        Lab7P1.c
 * Description: Connects and initializes an LCD to blink in the home position
 */
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
    /*************************************
     * Brief:   Initializes the LCD to 4 bit bus width, basic font,
     *              cursor returned to home and the displayed cleared
     *              with a blinking cursor.
     * Params:
     *          void
     * Returns:
     *          void
     */
    P4->DIR = 0xFF;             //Ensure all pins as outputs
    systick_delay(30);          //Beginning of provided mandatory init
    nibble_write(0x30, 0);
    systick_delay(10);
    nibble_write(0x30, 0);
    systick_delay(1);
    nibble_write(0x30, 0);
    systick_delay(1);
    nibble_write(0x20, 0);      //4 bit bus width selection
    systick_delay(1);           //End of provided mandatory init

    commandWrite(0x28);         //Data length, Display lines and Character font
    commandWrite(0x06);         //Cursor increment
    commandWrite(0x01);         //return home
    commandWrite(0x0F);         //Display on, cursor on blinking
}
void nibble_write(unsigned char data, unsigned char control)
{
    /*********************************************
     * Brief:   Takes in a 16 bit data set, then sends the upper 4 bits through D4-7.
     * Params:
     *          Unsigned char data:     actual info needing to reach LCD
     *          Unsigned char control:  Tells LCD if info is data or command
     * Returns:
     *          void
     */
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
    /*********************************************
     * Brief:   Sends commands to the LCD using nibble_write
     * Params:
     *          unsigned char command:  the 16 bit command to implement on the LCD
     * Returns:
     *          void
     */
    nibble_write(command & 0xF0, 0);        //Tells RS to be 0 to signal incomeing command, sends upper 4 bits
    nibble_write(command << 4, 0);          //RS = 0 and sends lower 4 bits
    systick_delay(4);
}
void dataWrite(unsigned char data)
{
    /********************************************
     * Brief:   Sends data using nibble_write
     * Params:
     *          unsigned char data: 16 bit info to be sent
     * Returns:
     *          void
     */
    nibble_write(data & 0xF0, RS);      //Tells RS = 1 to signal data incoming, sends upper 4 bits
    nibble_write(data << 4, RS);        //sends lower 4 bits
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
     * Brief:       Initializes all of P4 to outputs
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
