/**************************************************************************//**
 * @file     main.c
 * @brief    Mbed led blinky example
 *           
 * @author   Sergio Prado
 *           sergio.prado@e-labworks.com
 *
 ******************************************************************************/
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED1_GPIO  18
#define LED2_GPIO  20
#define LED3_GPIO  21
#define LED4_GPIO  23

#define LED_GPIO   LED2_GPIO

void init_hardware()
{
    /* set system tick for 1ms interrupt */
    SystemCoreClockUpdate();
    
    /* set led GPIO port */
    LPC_GPIO1->FIODIR |= (1<<LED1_GPIO);
    LPC_GPIO1->FIODIR |= (1<<LED2_GPIO);
    LPC_GPIO1->FIODIR |= (1<<LED3_GPIO);
    LPC_GPIO1->FIODIR |= (1<<LED4_GPIO);
}

void blink_led_task(void *pvParameters)
{
    while(1) {
        
        LPC_GPIO1->FIOSET = (1<<LED_GPIO);
        vTaskDelay(500/portTICK_RATE_MS);
	
        LPC_GPIO1->FIOCLR = (1<<LED_GPIO);
        vTaskDelay(500/portTICK_RATE_MS);
    }
}

int main(void)
{
    /* initialize hardware */
    init_hardware();
    
    /* create task to blink led */
    xTaskCreate(blink_led_task, (signed char *)"Blink led", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
    
    /* Start the scheduler. */
    vTaskStartScheduler();
    
    /* should never reach here! */
    for(;;);
}
