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
void port3_init();                          //Using 3.5,6,7 as button pins

float DC=0;
volatile uint8_t bFlag=0;

void main(void)
{
    port2_4_init();
    port3_init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    NVIC_EnableIRQ(PORT3_IRQn);
    __enable_interrupt();
    while(1){
        if(bFlag==1){
            DC = DC + (37537 * .10);
            TimerA0_1_init();
            bFlag = 0;
        }
        else if(bFlag==2){
            DC = DC - (37537 * .10);
            TimerA0_1_init();
            bFlag = 0;
        }
        else if(bFlag==3){
            DC = 0;
            TimerA0_1_init();
            bFlag = 0;
        }
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
    TIMER_A0->CCR[1] = DC;                                                               // DUTY CYCLE
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
void port3_init()
{
    P3->SEL0    &= ~0xE0;       //Clear both SEL for 3.5,6,7
    P3->SEL1    &= ~0xE0;
    P3->DIR     &= ~0xE0;       //Clear to input
    P3->REN     |=  0xE0;       //Enable resistors
    P3->OUT     |=  0xE0;       //Set to pull-up
    P3->IES     |=  0xE0;       //Set to falling edge i.e notice when low
    P3->IE      |=  0xE0;       //Enable interupts for 3.5,6,7
    P3->IFG     =   0;          //Clear interupt flags
}
void PORT3_IRQHandler()         //Port 3 ISR
{
    if(P3->IFG & 0x20){
        bFlag = 1;
        P3->IFG &= ~0x20;
    }
    if(P3->IFG & 0x40){
        bFlag = 2;
        P3->IFG &= ~0x40;
    }
    if(P3->IFG & 0x80){
        bFlag = 3;
        P3->IFG &= ~0x80;
    }
}
