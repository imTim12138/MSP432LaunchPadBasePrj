/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include "ISR_protype.h"
#include <stdint.h>
#include <stdbool.h>

/* Function Protype */
int main(void) 
{
	WDT_A_holdTimer();

	/* Configuring GPIO as an output */
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

	/* Configuring SysTick to trigger at 1500000
	*(MCLK is 3MHz so this will make it toggle every 0.5s) */
	SysTick_enableModule();
	SysTick_setPeriod(1500000);
	Interrupt_enableSleepOnIsrExit();
	SysTick_enableInterrupt();

	/* Enabling MASTER interrupts */
	Interrupt_enableMaster();

	while (1)
	{
		PCM_gotoLPM0();
	}
}
