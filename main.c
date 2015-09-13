/* DriverLib Includes -------------------------------------------------- */
	#include "driverlib.h"

/* Standard Includes --------------------------------------------------- */
	#include "ISR_protype.h"
	#include <stdint.h>
	#include <stdbool.h>

/* Timer_A Continuous Mode Configuration Parameter --------------------- */
	const Timer_A_ContinuousModeConfig continuousModeConfig = {
		TIMER_A_CLOCKSOURCE_ACLK,            // ACLK Clock Source
		TIMER_A_CLOCKSOURCE_DIVIDER_1,       // ACLK/1 = 32Khz
		TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
		TIMER_A_DO_CLEAR                     // Skip Clear Counter
	};

/* Timer_A Compare Configuration Parameter ----------------------------- */
	const Timer_A_CompareModeConfig compareConfig =	 {
		TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
		TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
		TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
		16000                                       // 16000 Period
	};

/* Variables ----------------------------------------------------------- */
	volatile uint16_t adcResult;
	uint32_t mchlValue;

/* Static Functions ---------------------------------------------------- */
	static void Prj_GPIO_Config(void);
	static void Prj_ADC14_Config(void);
	static void Prj_SysTick_Config(void);
	static void Prj_CS_Config(void);

/* Function Protype ---------------------------------------------------- */
int main(void) 
{
	/* Halting WDT */
	WDT_A_holdTimer();

	/* Driver Configurations */
	Prj_CS_Config();
	Prj_GPIO_Config();
	Prj_ADC14_Config();
	Prj_SysTick_Config();

	/* Enabling interrupts */
	Interrupt_enableSleepOnIsrExit();
	Interrupt_enableMaster();
	Interrupt_enableInterrupt(INT_ADC14);

	/* Triggering the start of the sample */
	ADC14_enableConversion();
	ADC14_toggleConversionTrigger();


	mchlValue = CS_getMCLK();
	while (1)
	{
		PCM_gotoLPM0();
	}
}

static void Prj_GPIO_Config(void)
{
	/* Configuring GPIO as an output LED*/
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	/* Setting up GPIO pins as analog inputs (and references) */
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
	GPIO_PIN6 | GPIO_PIN5 | GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
}

static void Prj_ADC14_Config(void)
{
   /* Initializing ADC (MCLK/1/1) */
   ADC14_enableModule();
   ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,0);

   /* Configuring ADC Memory (ADC_MEM0 A0/A1 Differential) in repeat mode
    * with use of external references */
   ADC14_configureSingleSampleMode(ADC_MEM0, true);
   ADC14_configureConversionMemory
	(ADC_MEM0, ADC_VREFPOS_EXTPOS_VREFNEG_EXTNEG,ADC_INPUT_A0, true);

   /* Enabling sample timer in auto iteration mode and interrupts*/
   ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
   ADC14_enableInterrupt(ADC_INT0);
}

static void Prj_SysTick_Config(void)
{
	/* Configuring SysTick to trigger at 1500000
	 * (MCLK is 3MHz so this will make it toggle every 0.5s)*/
	SysTick_enableModule();
	SysTick_setPeriod(1500000);
	SysTick_enableInterrupt();
}

static void Prj_CS_Config(void)
{
	/* Setting the external clock frequency. This API is optional, but will
	* come in handy if the user ever wants to use the getMCLK/getACLK/etc
	* functions
	*/
	CS_setExternalClockSourceFrequency(32000,CS_48MHZ);

	/* Starting HFXT in non-bypass mode without a timeout. Before we start
	* we have to change VCORE to 1 to support the 48MHz frequency */
	PCM_setCoreVoltageLevel(PCM_VCORE1);
	FlashCtl_setWaitState(FLASH_BANK0, 2);
	FlashCtl_setWaitState(FLASH_BANK1, 2);
	CS_startHFXT(false);

	/* Initializing MCLK to HFXT (effectively 48MHz) */
	CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
}
