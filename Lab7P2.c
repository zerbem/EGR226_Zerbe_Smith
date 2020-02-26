
/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 7- LCD interfacing
 * File:        Lab7P1.c
 * Description: Prints 4 lines worth of information onto the LCD.
 */
#include "msp.h"

#define RS 1         //4.0 mask
#define EN 4         //4.2 mask
char line1[14];
char line2[13];
char line3[3];
char line4[3];

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
    int i;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    line1[0] = 'M';
    line1[1] = 'A';
    line1[2] = 'R';
    line1[3] = 'S';
    line1[4] = 'H';
    line1[5] = 'A';
    line1[6] = 'L';
    line1[7] = 'L';
    line1[8] = ' ';
    line1[9] = 'Z';
    line1[10] ='E';
    line1[11] ='R';
    line1[12] ='B';
    line1[13] ='E';

    line2[0] = 'M';
    line2[1] = 'I';
    line2[2] = 'C';
    line2[3] = 'H';
    line2[4] = 'E';
    line2[5] = 'A';
    line2[6] = 'L';
    line2[7] = ' ';
    line2[8] = 'S';
    line2[9] = 'M';
    line2[10] ='I';
    line2[11] ='T';
    line2[12] ='H';

    line3[0] = 'E';
    line3[1] = 'G';
    line3[2] = 'R';

    line4[0] = '2';
    line4[1] = '2';
    line4[2] = '6';

    while(1){
        commandWrite(1);
        systick_delay(500);

        commandWrite(0x81);
        for(i=0;i<14;i++)
        {
            dataWrite(line1[i]);
        }
        commandWrite(0xC1);
        for(i=0;i<13;i++)
        {
            dataWrite(line2[i]);
        }
        commandWrite(0x97);
        for(i=0;i<3;i++)
        {
            dataWrite(line3[i]);
        }
        commandWrite(0xD7);
        for(i=0;i<3;i++)
        {
            dataWrite(line4[i]);
        }
        systick_delay(5000);
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
    nibble_write(command & 0xF0, 0);
    nibble_write(command << 4, 0);
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
    nibble_write(data & 0xF0, RS);      //Tells RS to be 0 to signal incomeing command, sends upper 4 bits
    nibble_write(data << 4, RS);        //RS = 0 and sends lower 4 bits
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
