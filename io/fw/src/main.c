/*
 * Author: Pawel Jablonski
 * E-mail: pj@xirx.net
 * WWW: xirx.net
 * GIT: git.xirx.net
 *
 * License: You can use this code however you like
 * but leave information about the original author.
 * Code is free for non-commercial and commercial use.
 */

#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <stm32f0xx_hal_rcc.h>
#include <system_stm32f0xx.h>

#include "type.h"
#include "led.h"
#include "rtc.h"
#include "keyboard.h"
#include "speaker.h"
#include "lcd.h"
#include "flash.h"
#include "io.h"
#include "rs232.h"
#include "usb.h"
#include "fs.h"

bool initIsEnded = FALSE; //!< Status of end initializing all devices

//! Init main clock
static void initClock(void)
{
	RCC_OscInitTypeDef oscInit;

	oscInit.OscillatorType = (RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE);
	oscInit.HSEState = RCC_HSE_ON;
	oscInit.HSICalibrationValue = 16;
	oscInit.LSEState = RCC_LSE_ON;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscInit.PLL.PLLMUL = RCC_PLL_MUL6;
	oscInit.PLL.PREDIV = RCC_PREDIV_DIV1;
	
	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK)
	{
		while(1);
	}
	
	RCC_ClkInitTypeDef clkInit;
	
	clkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
	clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkInit.APB1CLKDivider = RCC_HCLK_DIV1;
	
	if(HAL_RCC_ClockConfig(&clkInit, FLASH_LATENCY_1) != HAL_OK)
	{
		while(1);
	}

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//! Main clock ticks handler. It executes a fire function if the time counter reach maximum delay for connected device.
void SysTick_Handler(void)
{	
	HAL_IncTick();
	
	if(initIsEnded)
	{
		static unsigned long long time = 0;
		
		if((time % KEYBOARD_FIRE_DELAY) == 0) keyboardFire();
		if((time % IO_FIRE_CLOCK_DELAY) == 0) ioFireClock();
		if((time % IO_FIRE_PERIPHERIAL_DELAY) == 0) ioFirePeripherial();

		time++;
	}
}

//! Main entry function of the firmware
int main(void)
{
	HAL_Init();
	
	initClock();
	
	ioPreInit();
	
	ledInit();
	rtcInit();
	keyboardInit();
	speakerInit();
	lcdInit();
	flashInit();
	rs232Init();
	usbInit();
	fsInit();
	ioInit();
	
	initIsEnded = TRUE;
	
	lcdDrawLoop();
	
	// Do not allow returning from the main function
	while(TRUE);
		
	return(0);
}
