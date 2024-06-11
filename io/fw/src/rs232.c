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
#include <stm32f0xx_hal_uart.h>
#include <stm32f0xx_hal_usart.h>
#include <system_stm32f0xx.h>

#include "type.h"

#include "rs232.h"

#define RS232_BUFFER_RX_SIZE 64 //!< Receive buffer size
#define RS232_BUFFER_TX_SIZE 64 //!< Transmit buffer size

static unsigned char bufferRx[RS232_BUFFER_RX_SIZE]; //!< Receive circular buffer
static volatile unsigned int bufferRxStart = 0; //!< Receive data start position
static volatile unsigned int bufferRxEnd = 0; //!< Receive data end position

static unsigned char bufferTx[RS232_BUFFER_TX_SIZE]; //!< Transmit circular buffer
static volatile unsigned int bufferTxStart = 0; //!< Transmit data start position
static volatile unsigned int bufferTxEnd = 0; //!< Transmit data end position

static UART_HandleTypeDef uartHandle; //!< UART HAL handle

//! Init rs232 and set up UART
void rs232Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInit = {0};
	
	gpioInit.Pin = (GPIO_PIN_9 | GPIO_PIN_10);
	gpioInit.Mode = GPIO_MODE_AF_PP;
	gpioInit.Pull = GPIO_PULLUP;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInit.Alternate = GPIO_AF1_USART1;
	
	HAL_GPIO_Init(GPIOA, &gpioInit);
	
	__HAL_RCC_USART1_CLK_ENABLE();
	
	uartHandle.Instance = USART1;
	uartHandle.Init.BaudRate = 1200;
	uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	uartHandle.Init.StopBits = UART_STOPBITS_1;
	uartHandle.Init.Parity = UART_PARITY_NONE;
	uartHandle.Init.Mode = UART_MODE_TX_RX;
	uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	
	HAL_UART_Init(&uartHandle);
	__HAL_UART_ENABLE(&uartHandle);
	
	HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	HAL_UART_Receive_IT(&uartHandle, bufferRx, 1);
}

/**
 * Send char
 *
 * @param byte Char to send
 */
void rs232Send(unsigned char byte)
{
	__disable_irq();
	{
		if(bufferTxStart == bufferTxEnd)
		{
			bufferTx[bufferTxEnd] = byte;
			bufferTxEnd = ((bufferTxEnd + 1) % RS232_BUFFER_TX_SIZE);
			
			HAL_UART_Transmit_IT(&uartHandle, (bufferTx + bufferTxStart), 1);
		}
		else
		{
			if(bufferTxStart == ((bufferTxEnd + 1) % RS232_BUFFER_TX_SIZE))
			{
				__enable_irq();
				return;
			}
			
			bufferTx[bufferTxEnd] = byte;
			bufferTxEnd = ((bufferTxEnd + 1) % RS232_BUFFER_TX_SIZE);
		}
	}
	__enable_irq();
}

/**
 * Read a next char from receive buffer
 *
 * @return Next char from the receive buffer
 */
unsigned char rs232Receive(void)
{
	__disable_irq();
	{
		if(bufferRxStart == bufferRxEnd)
		{
			__enable_irq();
			return(0);
		}
		else
		{
			unsigned char byte = bufferRx[bufferRxStart];
			
			bufferRxStart = ((bufferRxStart + 1) % RS232_BUFFER_RX_SIZE);
			
			__enable_irq();
			return(byte);
		}
	}
}

/**
 * Get free space size in the send buffer
 *
 * @return Free space size in the send buffer
 */
unsigned char rs232GetSendBufferFree(void)
{
	unsigned char bufferFree = (RS232_BUFFER_TX_SIZE - 1);
	
	__disable_irq();
	{
		if(bufferTxStart < bufferTxEnd)
		{
			bufferFree -= (bufferTxEnd - bufferTxStart);
		}
		else if(bufferTxStart > bufferTxEnd)
		{
			bufferFree = ((bufferTxStart - bufferTxEnd) - 1);
		}
	}
	__enable_irq();
	
	return(bufferFree);
}

/**
 * Check if is a new data in the  buffer
 *
 * @return Status of waiting data in receive buffer
 */
bool rs232RxIsData(void)
{
	__disable_irq();
	if(bufferRxStart == bufferRxEnd)
	{
		__enable_irq();
		return(FALSE);
	}
	else
	{
		__enable_irq();
		return(TRUE);
	}
}

//! Take care of the UART IRQ event
void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uartHandle);
}

/**
 * Callback of the receive UART HAL event. Read a next char from the UART.
 *
 * @param handler UART HAL handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handler)
{
	if(handler->Instance == USART1)
	{
		__disable_irq();
		{
			if(bufferRxStart == ((bufferRxEnd + 1) % RS232_BUFFER_RX_SIZE))
			{
				bufferRxStart = ((bufferRxStart + 1) % RS232_BUFFER_RX_SIZE);
			}
			
			bufferRxEnd = ((bufferRxEnd + 1) % RS232_BUFFER_RX_SIZE);
			HAL_UART_Receive_IT(&uartHandle, (bufferRx + bufferRxEnd), 1);
		}
		__enable_irq();
	}
}

/**
 * Callback of the of the transmit UART HAL event. Write a next char to the UART.
 *
 * @param handler UART HAL handle
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handler)
{
	if(handler->Instance == USART1)
	{
		__disable_irq();
		{
			bufferTxStart = ((bufferTxStart + 1) % RS232_BUFFER_TX_SIZE);
			
			if(bufferTxStart == bufferTxEnd)
			{
				__enable_irq();
				return;
			}
			
			HAL_UART_Transmit_IT(&uartHandle, (bufferTx + bufferTxStart), 1);
		}
		__enable_irq();
	}
}
