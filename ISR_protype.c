/* DriverLib Includes -------------------------------------------------- */
	#include "driverlib.h"

/* Include itself ------------------------------------------------------ */
	#include "ISR_protype.h"
	#include <stdint.h>

/* Extern Variable Declare --------------------------------------------- */
	extern 	volatile uint16_t adcResult;

/* Interrupt Function Protype ------------------------------------------ */

/* toggle p1.1 LED in systick complete interrupt */
void systick_isr(void)
{
	GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void adc_isr(void)
{
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT0)
    {
        adcResult = ADC14_getResult(ADC_MEM0);
    }

}
