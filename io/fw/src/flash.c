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
#include <stm32f0xx_hal_spi.h>
#include <system_stm32f0xx.h>

#include "type.h"

#include "flash.h"

#define FLASH_TIMEOUT 100 //!< SPI HAL maximum time to process an operation

#define FLASH_REG_STATUS 0xD7 //!< Status register address for the flash

#define FLASH_STATUS_READY 0x80 //!< Ready status bit of the flash
#define FLASH_STATUS_PAGE_SIZE_512 0x01 //!< 512 bytes sector is set bit of the flash

#define FLASH_READ_SECTOR 0x53 //!< Read data from the flash to the internal buffer operation code
#define FLASH_WRITE_SECTOR 0x83 //!< Write data from the internal buffer to the flash operation code

#define FLASH_READ_BUFFER 0xd4 //!< Read data from the internal buffer operation code
#define FLASH_WRITE_BUFFER 0x84 //!< Write data to the internal buffer operation code

static void flashLock(void);
static void flashUnlock(void);

static unsigned char flashGetStatus(void);

static bool flashReadSector(unsigned int sector);
static bool flashWriteSector(unsigned int sector);

static bool flashReadBuffer(unsigned int sector, struct FlashSector *buffer);
static bool flashWriteBuffer(unsigned int sector, struct FlashSector *buffer);

static SPI_HandleTypeDef spiHandle; //!< SPI HAL handle

static volatile bool mutex; //!< Mutex lock for operations on the flash

//! Init flash and set up the SPI
void flashInit(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInit = {0};
	
	gpioInit.Pin = GPIO_PIN_13;
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOC, &gpioInit);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpioInit.Pin = (GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	gpioInit.Mode = GPIO_MODE_AF_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInit.Alternate = GPIO_AF0_SPI2;
	
	HAL_GPIO_Init(GPIOA, &gpioInit);
	
	__HAL_RCC_SPI1_CLK_ENABLE();
	
	spiHandle.Instance = SPI1;
	spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	spiHandle.Init.Mode = SPI_MODE_MASTER;
	spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
	spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
	spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
	spiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
	spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spiHandle.Init.CRCPolynomial = 7;
	spiHandle.Init.NSS = SPI_NSS_SOFT;
	
	HAL_SPI_Init(&spiHandle);
	__HAL_SPI_ENABLE(&spiHandle);
	
	while(1)
	{
		unsigned char status = flashGetStatus();
		
		if(!(status & FLASH_STATUS_READY))
		{
			continue;
		}
		
		if(!(status & FLASH_STATUS_PAGE_SIZE_512))
		{
			unsigned char buf[4] = { 0x3d, 0x2a, 0x80, 0xa6 };
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_SPI_Transmit(&spiHandle, buf, 4, FLASH_TIMEOUT);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		}
		else
		{
			break;
		}	
	}
}

/**
 * Read a sector data from the flash
 *
 * @param sector Sector number
 * @param buffer Buffer to write data from the flash
 *
 * @return Status of the operation
 */
bool flashRead(unsigned int sector, struct FlashSector *buffer)
{
	flashLock();
	
	if(sector > FLASH_SECTOR_QUANTITY)
	{
		return(FALSE);
	}
	
	if(!flashReadBuffer(sector, buffer))
	{
		flashUnlock();
		return(FALSE);
	}
	
	flashUnlock();

	return(TRUE);
}

/**
 * Write a sector data to the flash
 *
 * @param sector Sector number
 * @param buffer Buffer with data to write into the flash
 *
 * @return Status of the operation
 */
bool flashWrite(unsigned int sector, struct FlashSector *buffer)
{
	flashLock();
	
	if(sector > FLASH_SECTOR_QUANTITY)
	{
		return(FALSE);
	}
	
	if(!flashWriteBuffer(sector, buffer))
	{
		flashUnlock();
		return(FALSE);
	}
	
	flashUnlock();
	
	return(TRUE);
}

//! Lock access to the flash
static void flashLock(void)
{
	while(1)
	{
		__disable_irq();
		if(!mutex)
		{
			mutex = TRUE;
			
			__enable_irq();
			break;
		}
		__enable_irq();
		
		for(volatile int i = 0; i < 100; i++);
	}
}

//! Unlock access to the flash
static void flashUnlock(void)
{
	__disable_irq();
	{
		mutex = FALSE;
	}
	__enable_irq();
}

/**
 * Get status of the flash
 *
 * @return Status of the flash
 */
static unsigned char flashGetStatus(void)
{
	unsigned char address = FLASH_REG_STATUS;
	unsigned char status = 0;
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spiHandle, &address, 1, FLASH_TIMEOUT);
	HAL_SPI_Receive(&spiHandle, &status, 1, FLASH_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	return(status);
}

/**
 * Read a sector data from the flash to the internal buffer of the flash
 *
 * @param sector Sector number
 *
 * @return Status of the operation
 */
static bool flashReadSector(unsigned int sector)
{
	if(sector >= FLASH_SECTOR_QUANTITY)
	{
		return(FALSE);
	}
	
	unsigned char buf[4] = { FLASH_READ_SECTOR, (unsigned char)((sector >> 7) & 0x7f), (unsigned char)((sector << 1) & 0xfe), 0x00 };
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spiHandle, buf, 4, FLASH_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	return(TRUE);
}

/**
 * Write a sector data from the internal buffer of the flash to the flash
 *
 * @param sector Sector number
 *
 * @return Status of the operation
 */
static bool flashWriteSector(unsigned int sector)
{
	if(sector >= FLASH_SECTOR_QUANTITY)
	{
		return(FALSE);
	}
	
	unsigned char buf[4] = { FLASH_WRITE_SECTOR, (unsigned char)((sector >> 7) & 0x7f), (unsigned char)((sector << 1) & 0xfe), 0x00 };
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spiHandle, buf, 4, FLASH_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	return(TRUE);
}

/**
 * Read a sector data from the internal buffer of the flash
 *
 * @param sector Sector number
 * @param buffer Buffer to write data from the internal buffer of the flash
 *
 * @return Status of the operation
 */
static bool flashReadBuffer(unsigned int sector, struct FlashSector *buffer)
{
	if(sector >= FLASH_SECTOR_QUANTITY)
	{
		return(FALSE);
	}
	
	if(!flashReadSector(sector))
	{
		return(FALSE);
	}
	
	unsigned char buf[5] = { FLASH_READ_BUFFER, 0x00, 0x00, 0x00, 0x00 };
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spiHandle, buf, 5, FLASH_TIMEOUT);
	HAL_SPI_Receive(&spiHandle, buffer->data, FLASH_SECTOR_SIZE, FLASH_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	return(TRUE);
}

/**
 * Write a sector data into the internal buffer of the flash
 *
 * @param sector Sector number
 * @param buffer Buffer with data to write into the internal buffer of the flash
 *
 * @return Status of the operation
 */
static bool flashWriteBuffer(unsigned int sector, struct FlashSector *buffer)
{
	if(sector >= FLASH_SECTOR_QUANTITY)
	{
		return(FALSE);
	}
	
	unsigned char buf[4] = { FLASH_WRITE_BUFFER, 0x00, 0x00, 0x00 };
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spiHandle, buf, 4, FLASH_TIMEOUT);
	HAL_SPI_Transmit(&spiHandle, buffer->data, FLASH_SECTOR_SIZE, FLASH_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	
	while(!(flashGetStatus() & FLASH_STATUS_READY));
	
	if(!flashWriteSector(sector))
	{
		return(FALSE);
	}
	
	return(TRUE);
}
