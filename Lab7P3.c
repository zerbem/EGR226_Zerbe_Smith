/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 7- LCD interfacing
 * File:        Lab7P1.c
 * Description: Creates a scrolling marquee from home to off screen left
 *                  and back to home from the right in 1 seconds shifts.
 *                  Only shifts across the top line, address 00->0F.
 */
#include "msp.h"

#define RS 1         //4.0 mask
#define EN 4         //4.2 mask
char line1[15];     //character string for the printout

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

    line1[0] = 'L';
    line1[1] = 'A';
    line1[2] = 'B';
    line1[3] = 'R';
    line1[4] = 'A';
    line1[5] = 'T';
    line1[6] = 'O';
    line1[7] = 'R';
    line1[8] = 'Y';
    line1[9] = ' ';
    line1[10] ='O';
    line1[11] ='V';
    line1[12] ='E';
    line1[13] ='R';
    line1[14] =' ';

    commandWrite(1);
    systick_delay(500);



    while(1){

        int n = 1,j;                            //initializing int for counting
        commandWrite(0x80);                     //start at position 00
        commandWrite(0x06);
        for(i=0;i<15;i++)                       //Print "Labratory over to screen hold for 1 second
        {
            dataWrite(line1[i]);
        }
        systick_delay(1000);

        for(j=0;j<14;j++){                      //Reprint but skip letter n until all are gone
            commandWrite(0x01);                 //Reprint at home location
            systick_delay(50);                  //delay to get home
            for(i=n;i<14;i++){                  //begin at the nth letter
                dataWrite(line1[i]);
            }
            systick_delay(1000);                //wait a second
            n++;                                //Increase n after each print to skip the next letter
        }

        commandWrite(0x06);                     //increment printout
        commandWrite(0x8F);                     //Start from line 1 right
        for(i=0;i<15;i++){                      //print line1[]
            dataWrite(line1[i]);                //print the current ith letter
            systick_delay(1000);                //wait 1 second
            commandWrite(0x18);                 //shift display 1 to the left
        }
        commandWrite(0x01);                     //after back to original, clear display and restart
    }
}

void LCD_init()
{
    /*************************************
     * Brief:   Initializes the LCD to 4 bit bus width, basic font,
     *              cursor returned to home and the displayed cleared
     *              with a non-blinking cursor.
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
    commandWrite(0x0C);
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
    P4->OUT = data | control | EN;      //Pulse E to send through nibble
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
    nibble_write(command & 0xF0, 0);        //sends upper 4 bits and tells RS to be 0
    nibble_write(command << 4, 0);          //sends lower 4 bits and tells RS to be 0
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
