#include "msp.h"
#include "stdio.h"
#include "string.h"

/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 8- PWM via Timer A
 * File:        Lab8P3.c
 * Description: Creates a PWM with a duty cycle read through keypad
 *                  entries.
 */

void TimerA0_1_init();
void port2_4_init();
void port4_init();
void systick_init();
void systick_delay(uint16_t delaymS);
void find_DC();                          //prints the key pressed to CCS screen
uint8_t Read_keypad();                      //Reads the key pressed and debounces

int num, pressed;                           //GLOBAL variables to save the input. num represents col
float DUTYCYCLE=0;
int i=0;
float PIN[20];


void main(void)
{

    port2_4_init();
    port4_init();
    systick_init();

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    printf("Press keys on the keypad.\n");
    printf("USE LEADING ZEROS\n");
    while(i<=2){
        pressed = Read_keypad();            //look for a key press
        if(pressed)                         //if a key is pressed (1)
            find_DC();
        if(i==3){                                                                       //if 3 correct inputs
            DUTYCYCLE = 37537 * (((PIN[0]*100)+(PIN[1]*10)+PIN[2])/100);                //calculate duty cycle
            printf("CCR[1] = %lf\n",37537 * (((PIN[0]*100)+(PIN[1]*10)+PIN[2])/100));   //Error check duty cycle
            TimerA0_1_init();
        }
        systick_delay(10);                  //wait 10ms before looking for new keys
    }
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
     *          float DUTYCYCLE:    global variable to initialize the duty cycle
     * Returns:
     *          VOID
     */
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;         //up counting with SMCLK
    TIMER_A0->CCR[0] = 37537;                                                               //40 Hz period
    TIMER_A0->CCR[1] = DUTYCYCLE;                                                               // DUTY CYCLE
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
void port4_init()
{
    /*******************************
     * Brief:       Initializes all of P4 to inputs with 4.0,1,2,3 REN pull up
     * Params:
     *              Void
     * Returns:
     *              Void
     *****************************************************************/
    P4->SEL0 &= 0x00;           //Clearing all of P4 SELL
    P4->SEL1 &= 0x00;
    P4->DIR  &= 0x00;           //Clear all to inputs
    P4->REN  |= 0x0F;            //Enabling REN for 4.0,1,2,3
    P4->OUT  |= 0x0F;            //4.0,1,2,3 REN pulled up (1)
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
uint8_t Read_keypad()
{
    /**********************
     * Brief:   Cycles through setting each colomn pin to output low
     *              then checks if any of the columns went low. num is
     *              then worked through an algorithm to decide which button was pressed
     * Params:
     *          VOID
     * Returns:
     *          VOID
     */
    uint8_t col, row;

    for(col=0;col<3;col++){
        P4->DIR &= ~0x70; //Col to input (High impedance)
        P4->DIR |= BIT(4+col); //set col to output
        P4->OUT &= ~BIT(4+col); //Set to low to find keypress
        systick_delay(10);
        row = P4->IN & 0x0F;

        while(!(P4->IN&BIT0) | !(P4->IN&BIT1) | !(P4->IN&BIT2) | !(P4->IN&BIT3)); //wait for button release

        if(row!=0x0F) //If an input is low a key was pressed
            break;

    }
    P4->DIR &= ~0x70;
    if(col==3)
        return 0;

    if(row==0x0E)
        num = col+1; //num is global stores the pressed number, 1->3
    if(row==0x0D)
        num = 3+col+1; //4->6
    if(row==0x0B)
        num = 6+col+1; //7->9
    if(row==0x07)
        num = 9+col+1; //*,0,#
    return 1;
}
void find_DC()
{
    /**********************
     * Brief:   Uses num to create an integer array of only numbers
     *              sets the ith entry into PIN as long as num is a
     *              value that represents a number. Increments i each time
     * Params:
     *          int num:    global set in Read_keypad
     * Returns:
     *          VOID
     */
    //printf("i=%d\n",i);
    if(num >=1 && num<=9){           //if num is greater than 1 and less than 9 print num
        PIN[i] = num;
        printf("%g\n",PIN[i]);
    }
    else if(num==10){               //if num is 10 print '*'
        printf("INVALID. Restart input!\n");
        i=0;
    }
    else if(num==11){                //if num is 11 print '0'
        PIN[i] = 0;
        printf("%g\n",PIN[i]);
    }
    else if(num==12 && i<=3){        //if num is 12 print '#'
        printf("INVALID. Restart input!\n");
        i=0;
    }
    i++;

}
