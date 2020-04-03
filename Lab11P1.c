#include "msp.h"

/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 11- Precision timer and detection
 * File:        Lab11P1.c
 * Description: Sends and detects a 10Hz PWM through an IR LED.
 *                  When the IR LED is detected, a red LED will be lit
 *                  via a GPIO pin. This code uses TimerA0 and interrupts.
 */
void timerA0_init();
void timerA0_1_init();
void port2_5_init();
void port4_0_init();
void port2_4_init();

volatile int detect10Hz;
volatile uint16_t lastedge, currentedge, period;

void main(void)
{
    timerA0_init();
    timerA0_1_init();
    port2_5_init();
    port4_0_init();
    port2_4_init();

    NVIC_EnableIRQ(TA0_N_IRQn);
    __enable_irq();                                 //global interrupt enabled
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
while(1){
    if(detect10Hz){
        P4->OUT ^= 0x01;

        detect10Hz=0;
    }
    }
}
void timerA0_init()
{
    /*******************************
     * Brief:       Initializes TimerA0 with an 8 divider, capture mode
     *                  with interrupts enabled. CCTL 2 is used to detect
     *                  the IR frequency.
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    TIMER_A0->CTL    |= TIMER_A_CTL_TASSEL_2;        //SMCLK clock
    TIMER_A0->CTL    |= TIMER_A_CTL_MC__UP;          //UP mode
    TIMER_A0->CTL    |= TIMER_A_CTL_ID_3;            // x/8, 375KHz
    TIMER_A0->CTL    |= TIMER_A_CTL_CLR;             //clear TA0R

    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CM_1;          //Capture rising edge
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CCIS_0;        //CCI2A
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CCIE;          //Capture interrupt
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CAP;           //capture mode
    TIMER_A0->CCTL[2] | TIMER_A_CCTLN_SCS;           //synchronos capture
}
void port2_5_init()
{
    /*******************************
     * Brief:       Initializes P2.5 connected to TA0 and set to input. Hooked to
     *                  LED reciever
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    P2->SEL0 |= 0x20;                               //TA0.CCI2A input pin selection
    P2->SEL1 &= ~0x20;
    P2->DIR  &= ~0x20;                              //Input to recieve form timerA0
}
void port4_0_init()
{
    /*******************************
     * Brief:       Initializes P4.0 to an output for LED manipulation
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    P4->SEL0 &= ~0x01;
    P4->SEL1 &= ~0x01;
    P4->DIR  |=  0x01;
    P4->OUT  &= ~0x01;
}
void TA0_N_IRQHandler()                             //TA0 ISR
{
    /*******************************
     * Brief:       TA0 ISR. Captures two consecutive rising edges to calculate the period.
     *                  This is used to identify frequency. If the Hz is between 5% of 10Hz
     *                  a flag will be set and used in the main.
     * Params:
     *              Volatile uint16_t currentedge:  Captures the time of the current rising edge
     *              Volatile uint16_t lastedge:     Saves the previous time of the rising edge
     *              Volatile uint16_t period:       calculates period from current - last.
     * Returns:
     *              Void
     *****************************************************************/
    TIMER_A0->CCTL[2] &= ~1;                     //clear the TA.2 interrupt
    currentedge = TIMER_A0->CCR[2];
    period      = currentedge-lastedge;
    lastedge    = currentedge;
   if((35600 <= period)&&(period <= 39500))
        //P4->OUT ^= 0x01;
    detect10Hz = 1;
    TIMER_A0->CCTL[2] &= ~1;                    //Reclear flag to ensure it is not tripped during execution
}
void timerA0_1_init()
{
    /**********************
     * Brief:   initializes TimerA0_1 with the SMCLK clock in the up counter
     *              and in mode 7, reset set. Used for PWM of the IR LED transmitter
     *              it will send a signal in 10Hz intervals (0.1s).
     * Params:
     *          void
     * Returns:
     *          VOID
     */
    TIMER_A0->CTL    |= TIMER_A_CTL_SSEL__SMCLK;                                            //up counting with SMCLK
    TIMER_A0->CTL    |= TIMER_A_CTL_MC__UP;
    TIMER_A0->CTL    |= TIMER_A_CTL_ID_3;                                                   // x/8, 375KHz
    TIMER_A0->CTL    |= TIMER_A_CTL_CLR;                                                    //Clear TA0R
    TIMER_A0->CCR[0] = 37537;                                                               //10 Hz period
    TIMER_A0->CCR[1] = 37537/2;                                                             //Turn on the signal every 0.1s or 10Hz
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
