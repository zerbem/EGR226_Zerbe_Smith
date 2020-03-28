/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 10 - ADC peripheral and analog sensors
 * File:        Lab10P2.c
 * Description: Interfaces the on-board ADC with an external
 *                  temperature sensor, and prints the ambient
 *                  temperature to the console screen.
 */

#include "msp.h"
#include "stdio.h"

void ADCsetup();
void port5_5_init();
void systick_init();
void systick_delay(uint16_t delayms);

uint8_t tempC;
float tempF;

void main(void)
{
    ADCsetup();
    port5_5_init();
    systick_init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
                                 //start convertions
    NVIC_EnableIRQ(ADC14_IRQn);
    __enable_interrupt();

    while(1){
        ADC14->CTL0 |= 1;
        while(!ADC14->IFGR0);                           //while the flag is not set
        tempC = (ADC14->MEM[5])* .02 - 50;              //save value from memory into C
        tempF = (tempC * 1.8) + 32;                     //convert C into F
        printf("value in C is:\n%d\n", tempC);          //Print to screen
        printf("value in F is:\n%g\n", tempF);
        systick_delay(500);                             //delay to make it print every half second
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
