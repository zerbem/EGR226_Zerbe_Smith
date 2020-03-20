#include "msp.h"


/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 9- Interupts via GPIO and timers
 * File:        Lab9P2.c
 * Description: Increments the value shown of a seven segment LED
 *                  by one every second restarting after 9. Accomplished
 *                  with the SysTick timer interrupt.
 */

void systick_init_interrupt();
void port4_init();
void seg7Dispchange();

volatile int count=0;           //Global used to increment and check current value

void main(void)
{
    port4_init();
    systick_init_interrupt();
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	while(1){
	    seg7Dispchange();
	}
}
void systick_init_interrupt()
{
    /*******************************
     * Brief:       Initializes the SysTick timer to interrupts enabled
     *                  and to occur every 1 second
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    SysTick->CTRL = 0;
    SysTick->LOAD = 3000000;
    SysTick->VAL  = 0;
    SysTick->CTRL = 0x07;
}
void SysTick_Handler()
{
    /*******************************
     * Brief:       When called, increments count until 10 is reached,
     *                  it then resets count to 0
     * Params:
     *              volatile int count:     incremented and then recycled
     * Returns:
     *              Void
     *****************************************************************/
  ++count;
  if(count==10)
      count=0;
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
