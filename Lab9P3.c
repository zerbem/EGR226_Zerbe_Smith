#include "msp.h"


/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 9- Interupts via GPIO and timers
 * File:        Lab9P3.c
 * Description: Increments/ decrements the value shown on a seven segment LED
 *                  based on which button is pressed. Green for up blue for down
 *
 */

void port4_init();
void port3_init();
void seg7Dispchange();
void systick_init();
void systick_delay(uint16_t delaymS);


volatile int count=0;           //Global used to increment and check current value

void main(void)
{
    port4_init();
    port3_init();
    systick_init();
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    NVIC_EnableIRQ(PORT3_IRQn);
    __enable_interrupt();

	while(1){
	    seg7Dispchange();
	}
}
void port4_init()
{
    /*******************************
     * Brief:       Initializes P4.0,1,2,3,4,5,6 as outputs. Their output
     *                  set low to begin.
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    P4->SEL0 &= 0x00;           //Clearing all of P4 SELL
    P4->SEL1 &= 0x00;
    P4->DIR  |= 0x7F;           //Clear all to inputs
    P4->OUT  &= 0X00;           //Clear outputs for now
}
void port3_init()
{
    /**********************
     * Brief:   Initializes P3.5,6,7 as inputs with the internal resistor
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
        if((P3->IN & 0x20) == 0x00){
            if(count==9){
                count = 0;
            }
            else{
                count++;
            }
            P3->IFG &= ~0x20;
        }
    }
    if(P3->IFG & 0x40){
        systick_delay(5);                   //debounce routine
        if((P3->IN & 0x40) == 0x00)         //if pin still low
        if(count==0){
            count = 9;
        }
        else{
            count--;
        }
        P3->IFG &= ~0x40;
    }

}
void seg7Dispchange()
{
    /*******************************
     * Brief:       Increments the 7 seg LED by checking current
     *                  value of "count" and powering on the
     *                  corrosponding pin to the correct layout.
     * Params:
     *              volatile int count:     global incremented in the SysTick ISR
     *                                      and check in this function
     * Returns:
     *              Void
     *****************************************************************/
    if(count==0){
        P4->OUT = 0x5F;
    }
    if(count==1){
        P4->OUT = 0x06;
    }
    if(count==2){
        P4->OUT = 0x3B;
    }
    if(count==3){
        P4->OUT = 0x2F;
    }
    if(count==4){
        P4->OUT = 0x66;
    }
    if(count==5){
        P4->OUT = 0x6D;
    }
    if(count==6){
        P4->OUT = 0x7D;
    }
    if(count==7){
        P4->OUT = 0x07;
    }
    if(count==8){
        P4->OUT = 0x7F;
    }
    if(count==9){
        P4->OUT = 0x6F;
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
    SysTick->LOAD = 0x00FFFFFF;//15000 cycles
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
