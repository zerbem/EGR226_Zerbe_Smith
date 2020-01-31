#include "msp.h"


/**
 * main.c
 */

int debounceSwitch1();
void port1_1_init();
void port2_0_init();
void port2_1_init();
void port2_2_init();

void main(void)
{
    uint8_t var20, var21, var22;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    port1_1_init();
    port2_0_init();
    port2_1_init();
    port2_2_init();

    while(1){
        var20 = P2->OUT & 0x01; //Reading in the current output of P2.0-P2.2
        var21 = P2->OUT & 0x02;
        var22 = P2->OUT & 0x04;

        if(debounceSwitch1()==0){
            if(var20==0x00 && var21==0x00 && var22==0x00){ //if no lights on turn on RED
                P2->OUT |= 0x01;
            }
            else if(var20==0x01){ //if RED on turn on GREEN
                P2->OUT &= ~0x01;
                P2->OUT |= 0x02;
            }
            else if(var21==0x02){ //if GREEN on turn on BLUE
                P2->OUT &= ~0x02;
                P2->OUT |= 0x04;
            }
            else if(var22==0x04){ //if BLUE on turn on RED
                P2->OUT &= ~0x04;
                P2->OUT |= 0x01;
            }
        }

    }
}
    int debounceSwitch1()
    {
        int switchValue=1; //Switch is on until pressed

        while(P1IN & 0x02);

            __delay_cycles(15000); //Delay for 5 mS or 15000 cycles
            if((P1IN & 0x02)==0x00) //if pressed again
            {
                switchValue = 0; //change flag to pressed
            }
            __delay_cycles(15000);
        while((P1IN&0x02)==0x00);

        return switchValue; //Return 0 if pushed, 1 if not
    }
    void port1_1_init()
    {
        P1->SEL0 &= ~0x02; //clearing P1.1 SEL0 register for GPIO
        P1->SEL1 &= ~0x02; //clearing P1.1 SEL1 register for GPIO
        P1->DIR &= ~0x02;  //clearing P1.1 DIR register for input
        P1->REN |= 0x02;   //enabling internal resistor in P1.1
        P1->OUT |= 0x02;   //setting internal resistor in P1.1 to pull-up
    }
    void port2_0_init()
    {
        P2->SEL0 &= ~0x01; //clearing P2.0 SEL0 register for GPIO
        P2->SEL1 &= ~0x01; //clearing P2.0 SEL1 register for GPIO
        P2->DIR |= 0x01;  //set P2.0 DIR register for output
        P2->OUT &= ~0x01;   //clearing output of P2.0
    }
    void port2_1_init()
    {
        P2->SEL0 &= ~0x02; //clearing P2.1 SEL0 register for GPIO
        P2->SEL1 &= ~0x02; //clearing P2.1 SEL1 register for GPIO
        P2->DIR |= 0x02;  //set P2.1 DIR register for output
        P2->OUT &= ~0x02;   //clearing output of P2.1
    }
    void port2_2_init()
    {
        P2->SEL0 &= ~0x04; //clearing P2.2 SEL0 register for GPIO
        P2->SEL1 &= ~0x04; //clearing P2.2 SEL1 register for GPIO
        P2->DIR |= 0x04;  //set P2.2 DIR register for output
        P2->OUT &= ~0x04;   //clearing output of P2.2
    }
