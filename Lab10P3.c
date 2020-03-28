/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 10 - ADC peripheral and analog sensors
 * File:        Lab10P3.c
 * Description: Interfaces the on-board ADC with an external
 *                  temperature sensor, and prints the ambient
 *                  temperature to an LCD in either Celsius or Fehrenheit.
 *                  Selectable via push button caused interrupts.
 */

#include "msp.h"
#include "stdio.h"

#define RS 1         //4.0 mask
#define EN 4         //4.2 mask

char line1[16];
char Cline2[2];

void nibble_write(unsigned char data, unsigned char control);
void commandWrite(unsigned char command);
void dataWrite(unsigned char data);
void LCD_init();
void port4_init();
void ADCsetup();
void port5_5_init();
void systick_init();
void systick_delay(uint16_t delayms);
void inttochar(int tempC);
void PORT3_IRQHandler();
void port3_init();

uint8_t tempC, push=1;                  //Variable to save the ADC conversion into and flag for interrupts respectively
float tempF;                            //Variable to save the fehrenheit conversion
char TEMPC[3], TEMPF[3];                //arrays used to print to the LCD



void main(void)
{
    int i;

    ADCsetup();                         //initializations
    port4_init();
    port5_5_init();
    systick_init();
    LCD_init();
    port3_init();

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    NVIC_EnableIRQ(ADC14_IRQn);          //enabling interrupts
    NVIC_EnableIRQ(PORT3_IRQn);
    __enable_interrupt();



    line1[0] = 'C';                      //mandatory LCD first line print out
    line1[1] = 'u';
    line1[2] = 'r';
    line1[3] = 'r';
    line1[4] = 'e';
    line1[5] = 'n';
    line1[6] = 't';
    line1[7] = ' ';
    line1[8] = 'T';
    line1[9] = 'e';
    line1[10] ='m';
    line1[11] ='p';
    line1[12] ='.';
    line1[13] =' ';
    line1[14] ='i';
    line1[15] ='s';

    while(1){
        commandWrite(1);
        systick_delay(500);

        commandWrite(0x80);                             //printing out line1[]
        for(i=0;i<16;i++)
        {
            dataWrite(line1[i]);
        }

        ADC14->CTL0 |= 1;
        while(!ADC14->IFGR0);                           //while the flag is not set
        tempC = (ADC14->MEM[5])* .02 - 50;              //save value from memory into C
        tempF = (tempC * 1.8) + 32;                     //convert C into F

        sprintf(TEMPC, "%.1f",tempC);                   //convert the variables into LCD printable char arrays
        sprintf(TEMPF, "%.1f",tempF);
        commandWrite(0xC7);

    if(push == 1){                                      //checking which button was pushed
        for(i=0;i<=3;i++){                              //printing the TEMPC array
            dataWrite(TEMPC[i]);
        }

        dataWrite(0xDF);                                //degree symbol
        dataWrite('C');

    }

    if(push == 2){
        for(i=0;i<=3;i++){                              //printing the TEMPF array
            dataWrite(TEMPF[i]);
        }
        dataWrite(0xDF);
        dataWrite('F');
    }

        systick_delay(5000);                            //5 second delay of the LCD to improve legibility of the LCD

      /*  commandWrite(0x1F);
        */
    }
}


void ADCsetup()
{
    /**********************
     * Brief:   initializes the ADC with A0 and MEM 5 selected.
     * Params:
     *          VOID
     * Returns:
     *          VOID
     */
    ADC14->CTL0 =   0x10;                   //POWERED ON BUT DISABLED
    ADC14->CTL0 |=  0X04D80300;             //S/H MODE, MCLCK, 32 SAMPLE CLOCK, SW TRIG, 4 DIVISOR
    ADC14->CTL1 =   0X00000030;             //14 BIT RESOLUTION
    ADC14->MCTL[5]= 0;                      //A0 INPUT, SINGLE, VREF=AVCC
    ADC14->CTL1 |=  0X00050000;             //START CONVERTING @ MEM 5
    ADC14->CTL0 |=  2;                      //ENABLE ADC
}
void port5_5_init()
{
    /**********************
     * Brief:   initializes port 5.5 to be used as the ADC pin
     * Params:
     *          VOID
     * Returns:
     *          VOID
     */
    P5->SEL0 |= 0X20;                       //SET SEL0 AND SEL1 TO ENABLE ADC THROUGH A0.
    P5->SEL1 |= 0X20;                       //REFER TO PAGE 35 FOR TABLE
}
void systick_init()
{
    /**********************
     * Brief:   initializes the systick timer.
     * Params:
     *          VOID
     * Returns:
     *          VOID
     */
    SysTick->CTRL = 0x00;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0x00;
    SysTick->CTRL = 0x00000005;
}
void systick_delay(uint16_t delaymS)
{
    /**********************
     * Brief:   A delay function that uses the systick timer
     *              clocked with the 3 MHz master clock.
     * Params:
     *          uint16_t delaymS:   amount of miliseconds to
     *                                  delay for.
     * Returns:
     *          VOID
     */
    SysTick->LOAD = ((delaymS * 3000)-1);
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000)==0);
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


void port3_init()
{
    /**********************
     * Brief:   Initializes P3.5,6 as inputs with the internal resistor
     *              pulled up, and interrupts enabled at the falling edge
     * Params:
     *          void
     * Returns:
     *          VOID
     */
    P3->SEL0    &= ~0x60;       //Clear both SEL for 3.5,6
    P3->SEL1    &= ~0x60;
    P3->DIR     &= ~0x60;       //Clear to input
    P3->REN     |=  0x60;       //Enable resistors
    P3->OUT     |=  0x60;       //Set to pull-up
    P3->IES     |=  0x60;       //Set to falling edge i.e notice when low
    P3->IE      |=  0x60;       //Enable interupts for 3.5,6,7
    P3->IFG     =   0;          //Clear interupt flags
}
void PORT3_IRQHandler()         //Port 3 ISR
{
    /**********************
     * Brief:   ISR for port 3. Checks what pin triggered the interrupt
     *              and changes a global variable accordingly. Resets the IFG
     * Params:
     *          volatile uint8_t bFlag:     global flag changed accordingly
     * Returns:
     *          VOID
     */
    if(P3->IFG & 0x20){
        systick_delay(5);                   //debounce routine
        if((P3->IN & 0x20) == 0x00)
            push=1;
            P3->IFG &= ~0x20;

    }
    if(P3->IFG & 0x40){
        systick_delay(5);                   //debounce routine
        if((P3->IN & 0x40) == 0x00)         //if pin still low
        push = 2;
        P3->IFG &= ~0x40;
    }

}
