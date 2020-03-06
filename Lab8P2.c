#include "msp.h"
/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 8- PWM via Timer A
 * File:        Lab8P2.c
 * Description: Creates a PWM with a hardcoded duty cycle
 *                  written into CCR1
 */

void TimerA0_1_init();
void port2_4_init();

void main(void)
{
    TimerA0_1_init();
    port2_4_init();

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	while(1){
	    ;
	}
}

void TimerA0_1_init()
{
    /**********************
     * Brief:   initializes TimerA0_1 with the SMCLK clock in the up counter
     *              and in mode 7, reset set.
     * Params:
     *          void
     * Returns:
     *          VOID
     */
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;         //up counting with SMCLK
    TIMER_A0->CCR[0] = 37537;                                                               //40 Hz period
    TIMER_A0->CCR[1] = 3753;                                                               // DUTY CYCLE
    TIMER_A0->CCTL[1]= TIMER_A_CCTLN_OUTMOD_7;                                              //Output mode reset/set
}
void port2_4_init()
{
    /**********************
     * Brief:   initializes port 2.4 as a connection
     *              to TimerA0_1 via SEL0 set to 1
     * Params:
     *          void
     * Returns:
     *          VOID
     */
    P2->SEL0 |= 0x10;         //Set 2.4 SEl0 for connection to Timer A0.1
    P2->SEL1 &= ~0x10;        //Clear 2.4 SEL1
    P2->DIR  |= 0x10;         //Set to output
}

