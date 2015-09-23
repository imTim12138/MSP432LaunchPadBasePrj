/***********************************************************
*时钟配置：使用REFOCLK-128kHz配置HSMCLK--SMCLK输出500Hz
*Programmable UCxCLK frequency in master mode，先使用ACLK32768Hz-div64->512Hz
*实现之后改为外部时钟externally provided clock on the eUSCI_B SPI clock input pin
**/

/* DriverLib Includes -------------------------------------------------- */

/* Standard Includes --------------------------------------------------- */
	#include "ISR_protype.h"
	#include <stdint.h>
	#include <stdbool.h>

/* Variables ----------------------------------------------------------- */

/* Configuration structs ----------------------------------------------- */
