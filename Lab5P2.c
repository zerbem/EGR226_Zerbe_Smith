#include "msp.h"


/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 5- external connections
 * File:        Lab5P2.c
 * Description: Connects the MSP432 to three colored LEDs
 *                  activated through MOSFET 2N7000 transistor.
 *                  A button press will move the code forward onto
 *                  the next color. A button hold will continuously cycle
 *                  colors in 1 second intervals delayed by a systick timer.
 */

int debounceSwitch1();
void port1_7_init();
void port2_0_init();
void port2_1_init();
void port2_2_init();
void systick_init();
void systick_delay(uint16_t delaymS);

void main(void)
{
    uint8_t var20, var21, var22;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    port1_7_init();
    port2_0_init();
    port2_1_init();
    port2_2_init();
    systick_init();

    while(1){
        var20 = P2->OUT & 0x01; //RED
        var21 = P2->OUT & 0x02; //GREEN
        var22 = P2->OUT & 0x04; //YELLOW

        if(debounceSwitch1()==1){
            if(var20==0x00 && var21==0x00 && var22==0x00){ //if no lights on turn on GREEN
                P2->OUT |= 0x02;
                systick_delay(1000);
            }
            if(var21==0x02){ //if GREEN on turn on Yellow
                P2->OUT &= ~0x02;
                P2->OUT |= 0x04;
                systick_delay(1000);
            }
            if(var22==0x04){ //if Yellow on turn on RED
                P2->OUT &= ~0x04;
                P2->OUT |= 0x01;
                systick_delay(1000);
            }
            if(var20==0x01){ //if RED on turn on GREEN
                P2->OUT &= ~0x01;
                P2->OUT |= 0x02;
                systick_delay(1000);
            }
        }

    }
}
    int debounceSwitch1()
    {
        /**********************************
         * Brief:   debounces button 1 using systick_delay().
         *             Allows the button to be held for a continuous loop.
         * Params:
         *          VOID
         * Returns:
         *          int switchValue: Flag to verify if the button
         *                              was pressed.
         */
            int switchValue=0; //Switch is on until pressed

            while((P1IN & 0x80)==0); //while button is not pressed

                systick_delay(5); //Delay for 5 mS or 15000 cycles
                if(P1->IN) //if pressed again
                {
                    switchValue = 1; //change flag to pressed
                }
                //systick_delay(5);
            //while(P1IN & 0x80); //while the button is pressed

            return switchValue; //Return 1 if pushed, 0 if not
        }
    void port1_7_init()
    {
        /***********************
         * Brief:   Initializes P1.7 as an input with the internal
         *              resistor pulled down.
         * Params:
         *          VOID
         * Returns:
         *          VOID
         */
        P1->SEL0 &= ~0x80;
        P1->SEL1 &= ~0x80;
        P1->DIR &= ~0x80; //make 1.7 input
        P1->REN |= 0x80; //enable internal resistor
        P1->OUT &= 0x80; //clear REN to pull-down
    }
    void port2_0_init()
    {
        /***********************
         * Brief:   Initializes P2.0 as an output set to zero.
         * Params:
         *          VOID
         * Returns:
         *          VOID
         */
        P2->SEL0 &= ~0x01; //clearing P2.0 SEL0 register for GPIO
        P2->SEL1 &= ~0x01; //clearing P2.0 SEL1 register for GPIO
        P2->DIR |= 0x01;  //set P2.0 DIR register for output
        P2->OUT &= ~0x01;   //clearing output of P2.0
    }
    void port2_1_init()
    {
        /***********************
         * Brief:   Initializes P2.1 as an output set to zero.
         * Params:
         *          VOID
         * Returns:
         *          VOID
         */
        P2->SEL0 &= ~0x02; //clearing P2.1 SEL0 register for GPIO
        P2->SEL1 &= ~0x02; //clearing P2.1 SEL1 register for GPIO
        P2->DIR |= 0x02;  //set P2.1 DIR register for output
        P2->OUT &= ~0x02;   //clearing output of P2.1
    }
    void port2_2_init()
    {
        /***********************
         * Brief:   Initializes P2.2 as an output set to zero.
         * Params:
         *          VOID
         * Returns:
         *          VOID
         */
        P2->SEL0 &= ~0x04; //clearing P2.2 SEL0 register for GPIO
        P2->SEL1 &= ~0x04; //clearing P2.2 SEL1 register for GPIO
        P2->DIR |= 0x04;  //set P2.2 DIR register for output
        P2->OUT &= ~0x04;   //clearing output of P2.2
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
