#include "msp.h"
#include "stdio.h"

/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 6- Keypad reading and printing
 * File:        Lab6P1.c
 * Description: Connects/initializes/reads and prints input from a
 *                  ZRX-543 keypad. Can read key presses and print
 *                  the value pressed to the CCS monitor.
 */

void port4_init();
void systick_init();
void systick_delay(uint16_t delaymS);
void print_keys(); //prints the key pressed to CCS screen
uint8_t Read_keypad(); //Reads the key pressed and debounces

int num, pressed; //GLOBAL variables to save the input. num represents col

void main(void)
{
    port4_init();
    systick_init();

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    printf("Press keys on the keypad.\n");
    while(1){
        pressed = Read_keypad();            //look for a key press
        if(pressed)                         //if a key is pressed (1)
            print_keys();                   //print the keys
        systick_delay(10);                  //wait 10ms before looking for new keys
    }
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
void print_keys()
{
    if(num >=1 && num<=9)   //if num is greater than 1 and less than 9 print num
        printf("%d\n", num);
    else if(num==10)        //if num is 10 print '*'
        printf("*\n");
    else if(num==11)        //if num is 11 print '0'
        printf("0\n");
    else if(num==12)        //if num is 12 print '#'
        printf("#\n");
}
uint8_t Read_keypad()
{
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
