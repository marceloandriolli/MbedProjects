/**************************************************************************//**
 * @file     main.c
 * @brief    Mbed led blinky example
 *           
 * @author   Sergio Prado
 *           sergio.prado@e-labworks.com
 *
 ******************************************************************************/

#include "LPC17xx.h"

#define LED1_GPIO  18
#define LED2_GPIO  20
#define LED3_GPIO  21
#define LED4_GPIO  23

#define LED_GPIO   LED1_GPIO

volatile uint32_t msTicks;                            /* counts 1ms timeTicks */

void SysTick_Handler(void) 
{
    msTicks++;                        /* increment counter necessary in Delay() */
}

__INLINE static void Delay(uint32_t dlyTicks) 
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks);
}

int main (void) 
{
    /* set system tick for 1ms interrupt */
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000)) { 
        while (1);
    }

    /* set led GPIO port */
    LPC_GPIO1->FIODIR = (1<<LED_GPIO);
 
    /* blink led */
    while(1) {
        LPC_GPIO1->FIOSET = (1<<LED_GPIO);
        Delay (300);
        LPC_GPIO1->FIOCLR = (1<<LED_GPIO);
        Delay (300);
    }
}
