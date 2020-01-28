#include "msp.h"
#include <stdint.h>

int main(void) {
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                 WDT_A_CTL_HOLD;

    P1->DIR |= BIT0;                        // P1.0 set as output

    while (1)                               // continuous loop
    {
        P1->OUT ^= BIT0;                    // Blink P1.0 LED
        for (i = 20000; i > 0; i--);        // Delay
    }
}
