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
#include "font.h"

#include "lcd.h"

#define DEFAULT_COLOR 0x0f //!< Default foreground and background colors

#define COLOR_MASK_FOREGROUND 0x0f //!< Foreground color mask
#define COLOR_MASK_BACKGROUND 0xf0 //!< Background color mask
#define COLOR_SHIFT 4 //!< Char color shift, background color is high 4 bits, foreground color is low 4 bits

//! LCD buffer type list
enum LCDBufferID
{
	LCD_BUFFER_WORK = 0, //!< Work buffer
	LCD_BUFFER_READY, //!< Ready to display buffer
	LCD_BUFFER_DISPLAY, //!< Display buffer used to generate an image on the LCD
	LCD_BUFFER_QUANTITY //!< Quantity of the buffers
};

static volatile bool updateRequest = FALSE; //!< Status of update the screen request

static unsigned char cursorColor = DEFAULT_COLOR; //!< Current set foreground and background colors
static unsigned char cursorPosX = 0; //!< Current horizontal cursor position
static unsigned char cursorPosY = 0; //!< Current vertical cursor position

static unsigned char charBuffer[LCD_BUFFER_QUANTITY][LCD_HEIGHT][LCD_WIDTH]; //!< Char buffers
static unsigned char colorBuffer[LCD_BUFFER_QUANTITY][LCD_HEIGHT][LCD_WIDTH]; //!< Background and foreground color buffers

static void lcdDraw(void);
static void lcdDrawLine(int lineNumber);

//! Init LCD and set up buffers and output pins
void lcdInit(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInit = {0};
	
	gpioInit.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8);
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOC, &gpioInit);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(10);
	
	lcdClear();
	lcdRefresh();
}

//! Clear LCD work buffer
void lcdClear(void)
{
	for(int i = 0; i < LCD_HEIGHT; i++)
	{
		for(int j = 0; j < LCD_WIDTH; j++)
		{
			charBuffer[LCD_BUFFER_WORK][i][j] = 0;
			colorBuffer[LCD_BUFFER_WORK][i][j] = 0;
		}
	}
}

//! Refresh LCD ready buffer with data from the work buffer
void lcdRefresh(void)
{
	for(int i = 0; i < LCD_HEIGHT; i++)
	{
		for(int j = 0; j < LCD_WIDTH; j++)
		{
			charBuffer[LCD_BUFFER_READY][i][j] = charBuffer[LCD_BUFFER_WORK][i][j];
			colorBuffer[LCD_BUFFER_READY][i][j] = colorBuffer[LCD_BUFFER_WORK][i][j];
		}
	}
}

//! Scroll one line down data in the work buffer
void lcdScrollDown(void)
{
	for(int y = 0; y < (LCD_HEIGHT - 1); y++)
	{
		for(int x = 0; x < LCD_WIDTH; x++)
		{
			charBuffer[LCD_BUFFER_WORK][y][x] = charBuffer[LCD_BUFFER_WORK][y + 1][x];
			colorBuffer[LCD_BUFFER_WORK][y][x] = colorBuffer[LCD_BUFFER_WORK][y + 1][x];
		}
	}

	for(int x = 0; x < LCD_WIDTH; x++)
	{
		charBuffer[LCD_BUFFER_WORK][LCD_HEIGHT - 1][x] = 0;
		colorBuffer[LCD_BUFFER_WORK][LCD_HEIGHT - 1][x] = 0;
	}

	if(cursorPosY > 0)
	{
		cursorPosY--;
	}
}

//! Move position of cursor one line lower
void lcdNewLine(void)
{
	cursorPosX = 0;

	if(cursorPosY >= LCD_HEIGHT)
	{
		lcdScrollDown();
		cursorPosY = (LCD_HEIGHT - 1);
	}

	cursorPosY++;
}

/**
 * Set foreground color for next printed characters
 *
 * @param color Foreground color
 */
void lcdSetColorForeground(unsigned char color)
{
	cursorColor = ((cursorColor & COLOR_MASK_BACKGROUND) | (color & COLOR_MASK_FOREGROUND));
}

/**
 * Set background color for next printed characters
 *
 * @param color Background color
 */
void lcdSetColorBackground(unsigned char color)
{
	cursorColor = ((cursorColor & COLOR_MASK_FOREGROUND) | ((color << COLOR_SHIFT) & COLOR_MASK_BACKGROUND));
}

/**
 * Set new horizontal position for the cursor
 *
 * @param x New horizontal position of the cursor
 */
void lcdSetCursorPosX(unsigned char x)
{
	if(x < LCD_WIDTH)
	{
		cursorPosX = x;
	}
}

/**
 * Set new vertical position for the cursor
 *
 * @param y New vertical position of the cursor
 */
void lcdSetCursorPosY(unsigned char y)
{
	if(y < LCD_HEIGHT)
	{
		cursorPosY = y;
	}
}

/**
 * Get horizontal position of the cursor
 *
 * @return Horizontal position of the cursor
 */
unsigned char lcdGetCursorPosX(void)
{
	return(cursorPosX);
}

/**
 * Get vertical position of the cursor
 *
 * @return Vertical position of the cursor
 */
unsigned char lcdGetCursorPosY(void)
{
	return(cursorPosY);
}

/**
 * Print one character on the screen
 *
 * @param c Character to print
 */
void lcdPrintChar(unsigned char c)
{
	if(cursorPosX >= LCD_WIDTH)
	{
		cursorPosX = 0;
		cursorPosY++;
	}

	if(cursorPosY >= LCD_HEIGHT)
	{
		lcdScrollDown();
		cursorPosY = (LCD_HEIGHT - 1);
	}

	charBuffer[LCD_BUFFER_WORK][cursorPosY][cursorPosX] = c;
	colorBuffer[LCD_BUFFER_WORK][cursorPosY][cursorPosX] = cursorColor;

	cursorPosX++;
}

//! Set LCD update request flag to refresh the data on the screen
void lcdUpdate(void)
{
	updateRequest = TRUE;
}

//! Infinity draw loop on the LCD. It works at background, and it refreshes the LCD when no other operation is in progress.
void lcdDrawLoop(void)
{
	while(TRUE)
	{
		__disable_irq();
		if(updateRequest)
		{
			for(int i = 0; i < LCD_HEIGHT; i++)
			{
				for(int j = 0; j < LCD_WIDTH; j++)
				{
					charBuffer[LCD_BUFFER_DISPLAY][i][j] = charBuffer[LCD_BUFFER_READY][i][j];
					colorBuffer[LCD_BUFFER_DISPLAY][i][j] = colorBuffer[LCD_BUFFER_READY][i][j];
				}
			}
			
			updateRequest = FALSE;
			__enable_irq();
			
			lcdDraw();
		}
		else
		{
			__enable_irq();
		}
		
		for(volatile int i = 0; i < 100; i++);
	}
}

//! Draw a whole buffer on the LCD
static void lcdDraw(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	
	for(int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

	for(int i = 0; i < LCD_HEIGHT; i++)
	{
		lcdDrawLine(i);
	}
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
}

/**
 * Draw a one line on the LCD
 *
 * @param lineNumber Row number to draw on the LCD
 */
static void lcdDrawLine(int lineNumber)
{
	for(int i = 0; i < (LCD_CHAR_HEIGHT * 2); i++)
	{
		int row = lineNumber;
		int fontRow = (i / 2);
		
		for(int column = 0; column < LCD_WIDTH; column++)
		{
			unsigned char c = charBuffer[LCD_BUFFER_DISPLAY][row][column];
			unsigned char b = font[c][fontRow];
			
			unsigned char colorChar = (colorBuffer[LCD_BUFFER_DISPLAY][row][column] & 0x0f);
			unsigned char colorBackground = ((colorBuffer[LCD_BUFFER_DISPLAY][row][column] >> 4) & 0x0f);
			
			for(int k = 0; k < LCD_CHAR_WIDTH; k++)
			{
				if(b & 0x80)
				{
					GPIOC->ODR = ((GPIOC->ODR & 0xff00) | 0xa0 | colorChar);
				}
				else
				{
					GPIOC->ODR = ((GPIOC->ODR & 0xff00) | 0xa0 | colorBackground);
				}
				
				GPIOC->BSRR = 0x40;
				GPIOC->BRR = 0x40;
				GPIOC->BSRR = 0x40;
				
				b <<= 1;
			}
		}
	}
}
