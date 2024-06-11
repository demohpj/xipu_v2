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

//! Init LED and set up the pin outs
void ledInit(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInit = {0};
	
	gpioInit.Pin = (GPIO_PIN_8 | GPIO_PIN_15);
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &gpioInit);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	gpioInit.Pin = GPIO_PIN_9;
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOC, &gpioInit);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
}

/**
 * Set enable status of the run LED
 *
 * @param enable Enable status
 */
void ledSetRun(bool enable)
{
	if(enable)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	}
}

/**
 * Set enable status of the error LED
 *
 * @param enable Enable status
 */
void ledSetError(bool enable)
{
	if(enable)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	}
}

/**
 * Get enable status of the run LED
 *
 * @return Enable status
 */
bool ledGetRun(void)
{
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

/**
 * Get enable status of the error LED
 *
 * @return Enable status
 */
bool ledGetError(void)
{
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_SET)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

/**
 * Set enable status of the capslock LED
 *
 * @param enable Enable status
 */
void ledSetCapslock(bool enable)
{
	if(enable)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}
