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

#include "keyboard.h"

#define KEYBOARD_COLUMN_QUANTITY 15 //!< Quantity of key columns of keyboard
#define KEYBOARD_ROW_QUANTITY 5 //!< Quantity of key rows of keyboard

#define KEYBOARD_STATUS_RETRY 5 //!< How many times read status of all keys
#define KEYBOARD_STATUS_MIN 4 //!< How many statuses must be high to count a key as clicked

#define KEYBOARD_BUFFER_SIZE 16 //!< Global input buffer size
#define KEYBOARD_LOCAL_BUFFER_SIZE 8 //!< Local input buffer size for the read function

#define KEYBOARD_KEY_ESC 0x1b //!< Escape key code
#define KEYBOARD_KEY_BACKSPACE 0x08 //!< Backspace key code
#define KEYBOARD_KEY_TAB 0x09 //!< Tab key code
#define KEYBOARD_KEY_ENTER 0x0d //!< Enter key code
#define KEYBOARD_KEY_DELETE 0x7f //!< Delete key code
#define KEYBOARD_KEY_UP 0x11 //!< Up arrow key code
#define KEYBOARD_KEY_DOWN 0x12 //!< Down arrow key code
#define KEYBOARD_KEY_LEFT 0x13 //!< Left arrow key code
#define KEYBOARD_KEY_RIGHT 0x14 //!< Right arrow key code

#define KEYBOARD_CTRL_MASK 0x80 //!< Mask used to check if the control key is pressed too
#define KEYBOARD_CTRL_MIN_CODE 0x20 //!< Minimum key code where the control mask can be set

#define KEYBOARD_POS_SHIFT_LEFT_COLUMN 14 //!< Left shift key column position in the key statuses matrix
#define KEYBOARD_POS_SHIFT_LEFT_ROW 1 //!< Left shift key row position in the key statuses matrix
#define KEYBOARD_POS_SHIFT_RIGHT_COLUMN 3 //!< Right shift key column position in the key statuses matrix
#define KEYBOARD_POS_SHIFT_RIGHT_ROW 1 //!< Right shift key row position in the key statuses matrix
#define KEYBOARD_POS_CAPSLOCK_COLUMN 14 //!< Capslock key column position in the key statuses matrix
#define KEYBOARD_POS_CAPSLOCK_ROW 2 //!< Capslock key row position in the key statuses matrix
#define KEYBOARD_POS_CTRL_COLUMN 14 //!< Control key column position in the key statuses matrix
#define KEYBOARD_POS_CTRL_ROW 0 //!< Control key row position in the key statuses matrix

//! Keyboard matrix with data for every key for down and upper case
static const unsigned char keyCode[2][KEYBOARD_ROW_QUANTITY][KEYBOARD_COLUMN_QUANTITY] = {
	{
		{ 0x00, KEYBOARD_KEY_RIGHT, KEYBOARD_KEY_DOWN, KEYBOARD_KEY_LEFT, 0x00, 0x00, 0x00, 0x00, 0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0xff },
		{ 0x00, KEYBOARD_KEY_DELETE, KEYBOARD_KEY_UP, 0xff, '/', '.', ',', 'm', 'n', 'b', 'v', 'c', 'x', 'z', 0xff },
		{ 0x00, KEYBOARD_KEY_ENTER, 0x00, '\'', ';', 'l', 'k', 'j', 'h', 'g', 'f', 'd', 's', 'a', 0xff },
		{ 0x00, '\\', ']', '[', 'p', 'o', 'i', 'u', 'y', 't', 'r', 'e', 'w', 'q', KEYBOARD_KEY_TAB },
		{ KEYBOARD_KEY_BACKSPACE, '`', '+', '-', '0', '9', '8', '7', '6', '5', '4', '3', '2', '1', KEYBOARD_KEY_ESC }
	},
	{
		{ 0x00, KEYBOARD_KEY_RIGHT, KEYBOARD_KEY_DOWN, KEYBOARD_KEY_LEFT, 0x00, 0x00, 0x00, 0x00, 0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0xff },
		{ 0x00, KEYBOARD_KEY_DELETE, KEYBOARD_KEY_UP, 0xff, '?', '>', '<', 'M', 'N', 'B', 'V', 'C', 'X', 'Z', 0xff },
		{ 0x00, KEYBOARD_KEY_ENTER, 0x00, '"', ':', 'L', 'K', 'J', 'H', 'G', 'F', 'D', 'S', 'A', 0xff },
		{ 0x00, '|', '}', '{', 'P', 'O', 'I', 'U', 'Y', 'T', 'R', 'E', 'W', 'Q', KEYBOARD_KEY_TAB },
		{ KEYBOARD_KEY_BACKSPACE, '~', '+', '_', ')', '(', '*', '&', '^', '%', '$', '#', '@', '!', KEYBOARD_KEY_ESC },
	}
};

static bool keyLastStatus[KEYBOARD_ROW_QUANTITY][KEYBOARD_COLUMN_QUANTITY]; //!< Last key statuses matrix
static bool capslock = 0; //!< Capslock key status

static unsigned char buffer[KEYBOARD_BUFFER_SIZE]; //!< Circular keyboard input buffer
static volatile unsigned int bufferStart = 0; //!< Data start position
static volatile unsigned int bufferEnd = 0; //!< Data end position

//! Init keyboard and set up the pin outs and inputs
void keyboardInit(void)
{
	GPIO_InitTypeDef gpioInit = {0};
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	gpioInit.Pin = (GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOC, &gpioInit);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpioInit.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &gpioInit);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	gpioInit.Pin = GPIO_PIN_2;
	gpioInit.Mode = GPIO_MODE_INPUT;
	gpioInit.Pull = GPIO_PULLDOWN;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOD, &gpioInit);
	
	for(int i = 0; i < KEYBOARD_ROW_QUANTITY; i++)
	{
		for(int j = 0; j < KEYBOARD_COLUMN_QUANTITY; j++)
		{
			keyLastStatus[i][j] = FALSE;
		}
	}
}

//! Process the keyboard fire event. It reads current keyboard status and adds new data to the input buffer.
void keyboardFire(void)
{
	static unsigned char keyStatus[KEYBOARD_ROW_QUANTITY][KEYBOARD_COLUMN_QUANTITY];
	
	bool shift = FALSE;
	bool ctrl = FALSE;
	
	unsigned char localBuffer[KEYBOARD_LOCAL_BUFFER_SIZE];
	unsigned char localBufferEnd = 0;
	
	for(int i = 0; i < KEYBOARD_ROW_QUANTITY; i++)
	{
		for(int j = 0; j < KEYBOARD_COLUMN_QUANTITY; j++)
		{
			keyStatus[i][j] = 0;
		}
	}
	
	for(int retry = 0; retry < KEYBOARD_STATUS_RETRY; retry++)
	{
		for(int i = 0; i < KEYBOARD_ROW_QUANTITY; i++)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, ((i & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET));
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, ((i & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET));
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, ((i & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET));
			
			for(int j = 0; j < KEYBOARD_COLUMN_QUANTITY; j++)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, ((j & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET));
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, ((j & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET));
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, ((j & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET));
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, ((j & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET));
				
				// wait
				{
					volatile int w = 15;
					while(w--);
				}
				
				if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_SET)
				{
					keyStatus[i][j]++;
				}
			}
		}
	}
	
	if(keyStatus[KEYBOARD_POS_SHIFT_LEFT_ROW][KEYBOARD_POS_SHIFT_LEFT_COLUMN] >= KEYBOARD_STATUS_MIN)
	{
		shift = TRUE;
	}
	else if(keyStatus[KEYBOARD_POS_SHIFT_RIGHT_ROW][KEYBOARD_POS_SHIFT_RIGHT_COLUMN] >= KEYBOARD_STATUS_MIN)
	{
		shift = TRUE;
	}
	
	if(keyStatus[KEYBOARD_POS_CAPSLOCK_ROW][KEYBOARD_POS_CAPSLOCK_COLUMN] >= KEYBOARD_STATUS_MIN)
	{
		if(!(keyLastStatus[KEYBOARD_POS_CAPSLOCK_ROW][KEYBOARD_POS_CAPSLOCK_COLUMN]))
		{
			if(capslock)
			{
				ledSetCapslock(FALSE);
				capslock = FALSE;
			}
			else
			{
				ledSetCapslock(TRUE);
				capslock = TRUE;
			}
			
			keyLastStatus[KEYBOARD_POS_CAPSLOCK_ROW][KEYBOARD_POS_CAPSLOCK_COLUMN] = TRUE;
		}
	}
	else
	{
		keyLastStatus[KEYBOARD_POS_CAPSLOCK_ROW][KEYBOARD_POS_CAPSLOCK_COLUMN] = FALSE;
	}
	
	if(keyStatus[KEYBOARD_POS_CTRL_ROW][KEYBOARD_POS_CTRL_COLUMN] >= KEYBOARD_STATUS_MIN)
	{
		ctrl = TRUE;
	}
	
	for(int i = 0; i < KEYBOARD_ROW_QUANTITY; i++)
	{
		for(int j = 0; j < KEYBOARD_COLUMN_QUANTITY; j++)
		{
			if((keyCode[0][i][j] > 0x00) && (keyCode[0][i][j] < 0xff))
			{
				if(keyStatus[i][j] >= KEYBOARD_STATUS_MIN)
				{
					if(!(keyLastStatus[i][j]))
					{
						unsigned char key;
						
						if(((!capslock) && shift) || (capslock && (!shift)))
						{
							key = keyCode[1][i][j];
						}
						else
						{
							key = keyCode[0][i][j];
						}
						
						if(ctrl && (key >= KEYBOARD_CTRL_MIN_CODE))
						{
							key |= KEYBOARD_CTRL_MASK;
						}
						
						if(localBufferEnd < KEYBOARD_LOCAL_BUFFER_SIZE)
						{
							localBuffer[localBufferEnd] = key;
							
							localBufferEnd++;
						}
						
						keyLastStatus[i][j] = TRUE;
					}
				}
				else
				{
					keyLastStatus[i][j] = FALSE;
				}
			}
		}
	}
	
	for(int i = 0; i < localBufferEnd; i++)
	{
		__disable_irq();
		{
			if(bufferStart == ((bufferEnd + 1) % KEYBOARD_BUFFER_SIZE))
			{
				__enable_irq();
				break;
			}
			
			buffer[bufferEnd] = localBuffer[i];
			bufferEnd = ((bufferEnd + 1) % KEYBOARD_BUFFER_SIZE);
		}
		__enable_irq();
	}
}

/**
 * Read a next char from input buffer
 *
 * @return Next char from the input buffer
 */
unsigned char keyboardGetKey(void)
{
	__disable_irq();
	if(bufferStart == bufferEnd)
	{
		__enable_irq();
		return(0x00);
	}
	else
	{
		unsigned int first = bufferStart;
		
		bufferStart = ((first + 1) % KEYBOARD_BUFFER_SIZE);
		
		unsigned char val = buffer[first];
		
		__enable_irq();
		return(val);
	}
}

/**
 * Check if is a new data in the input buffer
 *
 * @return Status of waiting data in the input buffer
 */
bool keyboardIsData(void)
{
	__disable_irq();
	if(bufferStart == bufferEnd)
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
