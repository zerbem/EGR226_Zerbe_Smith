#include "msp.h"

/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 8- PWM via Timer A
 * File:        Lab8P1.c
 * Description: Creates a suedo PWM system using port 2.4
 *                  and a delay to toggle between on and off cycles
 */
void systick_init();
void systick_delay(uint16_t delaymS);
void port2_4_init();

int DC_off = 25;                        //Percentage of 100 to be off
int DC_on = 75;                         //Percentage of 100 to be on

void main(void)
{
    port2_4_init();
    systick_init();
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	while(1){
	    systick_delay(DC_off);          //period off
	    P2->OUT ^= 0x10;                //Toggle on 2.4
	    systick_delay(DC_on);           //period on
	    P2->OUT ^= 0x10;                //toggle off 2.4
	}

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
    SysTick->VAL  = 0x00;
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
void port2_4_init()
{
    /**********************
     * Brief:   initializes port 2.4 as an output
     *              to be toggled in the main
     * Params:
     *          void
     * Returns:
     *          VOID
     */
    P2->SEL0 &= ~0x10;       //Clear 2.4 SEl
    P2->SEL1 &= ~0x10;
    P2->DIR  |= 0x10;        //Set to output
    P2->OUT  &= ~0x10;       //Clear output
}
