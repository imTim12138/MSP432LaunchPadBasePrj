/* DriverLib Includes */
#include "driverlib.h"

/* Include itself */
#include "ISR_protype.h"
#include <stdint.h>

/* Interrupt Function Protype */

/* toggle p1.1 LED in systick complete interrupt */
void systick_isr(void)
{
	GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

