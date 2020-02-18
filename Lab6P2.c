#include "msp.h"
#include "stdio.h"
#include "string.h"

/*
 * Name:        Marshall Zerbe, Micheal Smith
 * Course:      EGR 226-901
 * Project:     Lab 6- Keypad reading and printing
 * File:        Lab6P2.c
 * Description: Like P1
 */

void port4_init();
void systick_init();
void systick_delay(uint16_t delaymS);
void save_string();                          //prints the key pressed to CCS screen
uint8_t Read_keypad();                      //Reads the key pressed and debounces

int num, pressed;                           //GLOBAL variables to save the input. num represents col
int j=1,i=0;
int PIN[20];                                 //GLOBAL array for use in functions

void main(void)
{
    port4_init();
    systick_init();

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    printf("Press keys on the keypad.\n");
    while(1){
        pressed = Read_keypad();            //look for a key press
        if(pressed)                         //if a key is pressed (1)
            save_string();
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
void save_string()
{
    //int a;                        //Used to count inputs

    if(num >=1 && num<=9){           //if num is greater than 1 and less than 9 print num
        PIN[i] = num;
        printf("%d\n",num);
    }
    else if(num==10){               //if num is 10 print '*'
        printf("INVALID. Restart input!\n");
        i=0;
        j=1;
    }
    else if(num==11){                //if num is 11 print '0'
        PIN[i] = num;
        printf("%d\n",num);
    }
    else if(num==12 && i<=3){        //if num is 12 print '#'
        printf("INVALID. Restart input!\n");
        i=0;
        j=1;
    }
    else if(num==12 && i>3){
        printf("Finalized code:\n");
        printf("%d%d%d%d\n",PIN[j-5],PIN[j-4],PIN[j-3],PIN[j-2]);
        /*for(a=j-3;a<=j;a++){
            printf("%d",PIN[j]);
        }*/
    }
    j++;                            //used to print PIN array
    i++;
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
