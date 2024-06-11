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
#include "keyboard.h"
#include "led.h"
#include "lcd.h"
#include "rs232.h"
#include "rtc.h"
#include "speaker.h"
#include "fs.h"

#include "io.h"

#define HALF_LOW FALSE //!< Low half of data transfer
#define HALF_HIGH TRUE //!< High half of data transfer

#define MODE_REG_SELECT FALSE //!< Register select mode
#define MODE_DATA_TRANSFER TRUE //!< Data transfer mode

#define RW_READ FALSE //!< Read data mode
#define RW_WRITE TRUE //!< Write data mode

//! Register addresses
enum RegAddress
{
	REG_COMMAND = 0, //!< Command register
	REG_STATUS = 1, //!< Status register
	REG_FIELD_ADDRESS = 2, //!< Field address register
	REG_FIELD = 3, //!< Field data register
	REG_KEYBOARD = 4, //!< Keyboard register
	REG_LCD_COLOR = 5, //!< LCD color register
	REG_LCD_CURSOR_POS_X = 6, //!< LCD cursor horizontal position register
	REG_LCD_CURSOR_POS_Y = 7, //!< LCD cursor vertical position register
	REG_LCD_CHAR = 8, //!< LCD char register
	REG_RS232 = 9, //!< RS232 register
	REG_FS = 10 //!< File System register
};

//! Low part commands
enum CommandLow
{
	FS_OPEN_OS = 0, //!< Open OS file command
	FS_RESET_NAME = 1, //!< Reset name buffer command
	FS_OPEN_APP = 2, //!< Open an application command
	FS_OPEN_FILE = 3, //!< Open a file command
	FS_LIST_APP = 4, //!< List files in the app directory
	FS_LIST_FILE = 5 //!< List files in the file sirectory
};

//! High part commands
enum CommandHigh
{
	LED_RUN_OFF = 0, //!< Turn off the run LED
	LED_RUN_ON = 1, //!< Turn on the run LED
	LED_ERROR_OFF = 2, //!< Turn off the error LED
	LED_ERROR_ON = 3, //!< Turn on the error LED
	LCD_CLEAR = 4, //!< Clear the LCD content
	LCD_REFRESH = 5, //!< Refresh the LCD content
	LCD_SCROLL_DOWN = 6, //!< Scroll down one line the LCD content
	LCD_NEW_LINE = 7, //!< Move the cursor one position down
	RTC_WRITE = 8, //!< Write the date and time buffer to the RTC
	RTC_READ = 9, //!< Read from the RTC to the date and time buffer
	SPEAKER_PLAY = 11, //!< Start playing sound
	SPEAKER_PAUSE = 12, //!< Pause playing sound
	SPEAKER_CLEAR = 13, //!< Clear the sound buffer
	SPEAKER_ADD_NOTE = 14, //!< Add a note to the sound buffer
	SPEAKER_GET_BUFFER_FREE = 15 //!< Get a free space size in the sound buffer
};

//! Low part statuses
enum StatusLow
{
	FS_STATUS = 0 //!< Status of the last FS operation
};

//! High part statuses
enum StatusHigh
{
	LED_RUN_ENABLED = 0, //!< Status of enable the run LED
	LED_ERROR_ENABLED = 1, //!< Status of enable the error LED
	SPEAKER_IS_PLAYING = 2 //!< Status of playing
};

//! Readable and writable fields
enum FieldRW
{
	RTC_YEAR = 4, //!< RTC year field
	RTC_MONTH = 5, //!< RTC month field
	RTC_DAY = 6, //!< RTC day field
	RTC_HOUR = 7, //!< RTC hour field
	RTC_MINUTE = 8, //!< RTC minute field
	RTC_SECOND = 9 //!< RTC second field
};

//! Writable fields
enum FieldW
{
	SPEAKER_NOTE = 12, //!< Code of a note to play field
	SPEAKER_TIME = 13, //!< Play time of a note field
	SPEAKER_FILL = 14, //!< Square fill of a note field
	SPEAKER_VOLUME = 15 //!< Volume of a note field
};

//! Readable fields
enum FieldR
{
	FS_DATA_SIZE_LOW = 0, //!< Low byte of a file size field
	FS_DATA_SIZE_HIGH = 1, //!< High byte of a file size field
	SPEAKER_BUFFER_FREE = 12 //!< Free space size in the sound buffer field
};

static unsigned char regSelected = 0; //!< Current selected register

//!< This struct contains motherboard's virtual registers buffer
struct
{
	unsigned char statusAddress; //!< Select a status register
	unsigned char fieldAddress; //!< Select a field register
	struct RTCDateTime rtc; //!< RTC data buffer
	struct
	{
		struct SpeakerNote note; //!< Note parameters buffer
		unsigned char bufferFree; //!< Free space size in the sound buffer
	} speaker; //!< Speaker data buffer
	struct
	{
		char name[FS_PATH_MAX_LENGTH + 1]; //!< Path buffer
		unsigned char nameLength; //!< Length of the path buffer
		struct FSSize size; //!< Size buffer
		bool status; //!< Status buffer
	} fs; //!< File System buffer
} static reg = {0};

static unsigned char ioReadStatus(bool half);
static unsigned char ioReadField(void);
static unsigned char ioReadKeyboard(bool half);
static unsigned char ioReadLCDCursorPosX(bool half);
static unsigned char ioReadLCDCursorPosY(bool half);
static unsigned char ioReadRS232Rx(bool half);
static unsigned char ioReadFSData(bool half);

static void ioWriteCommand(bool half, unsigned char dataInRaw);
static void ioWriteStatusAddress(unsigned char dataInRaw);
static void ioWriteFieldAddress(unsigned char dataInRaw);
static void ioWriteField(bool half, unsigned char dataIn, unsigned char mask);
static void ioWriteLCDColor(bool half, unsigned char dataInRaw);
static void ioWriteLCDCursorPosX(bool half, unsigned char dataIn, unsigned char mask);
static void ioWriteLCDCursorPosY(bool half, unsigned char dataIn, unsigned char mask);
static void ioWriteLCDChar(bool half, unsigned char dataIn, unsigned char mask);
static void ioWriteCRS232Tx(bool half, unsigned char dataIn, unsigned char mask);
static void ioWriteFSName(bool half, unsigned char dataIn, unsigned char mask);

static void ioCreatePath(char *buffer, const char *dirName, const char *fileName);

//! First stage of init. It set up all IO pins. It must be executed before all devices init.
void ioPreInit(void)
{
	GPIO_InitTypeDef gpioInit = {0};
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	gpioInit.Pin = (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
	gpioInit.Mode = GPIO_MODE_INPUT;
	gpioInit.Pull = GPIO_PULLDOWN;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpioInit);
	
	gpioInit.Pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpioInit);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

//! Second stage of init. It set up the IRQ used in IO communication. I can only be executed when all devices are inited.
void ioInit(void)
{
	GPIO_InitTypeDef gpioInit = {0};
	
	gpioInit.Pin = GPIO_PIN_15;
	gpioInit.Mode = GPIO_MODE_IT_RISING;
	gpioInit.Pull = GPIO_PULLDOWN;
	gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &gpioInit);

	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

//! Process the IO clock fire event. Toggle the clock pin.
void ioFireClock(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}

//! Process the IO peripherial fire event. Check if the keyboard or the rs232 have a new data in the buffers.
void ioFirePeripherial(void)
{
	if(keyboardIsData() == TRUE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	}
	
	if(rs232RxIsData() == TRUE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	}
}

/**
 * Read a value of the selected status register
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char ioReadStatus(bool half)
{
	unsigned char data = 0;

	if(half == HALF_LOW)
	{
		switch(reg.statusAddress)
		{
			case FS_STATUS :
				if(reg.fs.status)
				{
					data = 1;
				}
				break;
		}
	}
	else
	{
		switch(reg.statusAddress)
		{
			case LED_RUN_ENABLED :
				if(ledGetRun())
				{
					data = 1;
				}
				break;

			case LED_ERROR_ENABLED :
				if(ledGetError())
				{
					data = 1;
				}
				break;

			case SPEAKER_IS_PLAYING :
				if(speakerIsPlaying())
				{
					data = 1;
				}
				break;
		}

	}

	return(data);
}

/**
 * Read a value of the selected field register
 *
 * @return Read data
 */
unsigned char ioReadField(void)
{
	unsigned char data = 0;

	switch(reg.fieldAddress)
	{
		case FS_DATA_SIZE_LOW :
			data = reg.fs.size.low;
			break;

		case FS_DATA_SIZE_HIGH :
			data = reg.fs.size.high;
			break;

		case SPEAKER_BUFFER_FREE :
			data = reg.speaker.bufferFree;
			break;

		case RTC_YEAR :
			data = reg.rtc.year;
			break;

		case RTC_MONTH :
			data = reg.rtc.month;
			break;

		case RTC_DAY :
			data = reg.rtc.day;
			break;

		case RTC_HOUR :
			data = reg.rtc.hour;
			break;

		case RTC_MINUTE :
			data = reg.rtc.minute;
			break;

		case RTC_SECOND :
			data = reg.rtc.second;
			break;
	}

	return(data);
}

/**
 * Read a next character from the keyboard
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char ioReadKeyboard(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = keyboardGetKey();
		
		if(keyboardIsData() == FALSE)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		}
	}

	return(data);
}

/**
 * Read a horizontal position of the cursor
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char ioReadLCDCursorPosX(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = lcdGetCursorPosX();
	}

	return(data);
}

/**
 * Read a vertical position of the cursor
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char ioReadLCDCursorPosY(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = lcdGetCursorPosY();
	}

	return(data);
}

/**
 * Read a next byte from the RS232
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char ioReadRS232Rx(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = rs232Receive();
		
		if(rs232RxIsData() == FALSE)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		}
	}

	return(data);
}

/**
 * Read a next byte from the File System data buffer
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char ioReadFSData(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = fsReadNextDataByte();
	}

	return(data);
}

/**
 * Execute a command
 *
 * @param half Half of register selector
 * @param dataInRaw Command to execute
 */
void ioWriteCommand(bool half, unsigned char dataInRaw)
{
	if(half == HALF_LOW)
	{
		static char path[FS_PATH_MAX_LENGTH + 1];
		
		switch(dataInRaw)
		{
			case FS_OPEN_OS :
				reg.fs.status = fsOpen("/os.bin", &(reg.fs.size));
				break;

			case FS_RESET_NAME :
				reg.fs.name[0] = 0x00;
				reg.fs.nameLength = 0;
				break;

			case FS_OPEN_APP :
				ioCreatePath(path, "/app", reg.fs.name);
				reg.fs.status = fsOpen(path, &(reg.fs.size));
				break;

			case FS_OPEN_FILE :
				ioCreatePath(path, "/file", reg.fs.name);
				reg.fs.status = fsOpen(path, &(reg.fs.size));
				break;

			case FS_LIST_APP :
				reg.fs.status = fsList("/app", &(reg.fs.size));
				break;

			case FS_LIST_FILE :
				reg.fs.status = fsList("/file", &(reg.fs.size));
				break;
		}
	}
	else
	{
		switch(dataInRaw)
		{
			case LED_RUN_OFF :
				ledSetRun(FALSE);
				break;

			case LED_RUN_ON :
				ledSetRun(TRUE);
				break;

			case LED_ERROR_OFF :
				ledSetError(FALSE);
				break;

			case LED_ERROR_ON :
				ledSetError(TRUE);
				break;

			case LCD_CLEAR :
				lcdClear();
				lcdRefresh();
				lcdUpdate();
				break;

			case LCD_REFRESH :
				lcdRefresh();
				lcdUpdate();
				break;

			case LCD_SCROLL_DOWN :
				lcdScrollDown();
				break;

			case LCD_NEW_LINE :
				lcdNewLine();
				break;

			case RTC_WRITE :
				rtcSetDateTime(reg.rtc);
				break;

			case RTC_READ :
				reg.rtc = rtcGetDateTime();
				break;

			case SPEAKER_PLAY :
				speakerPlay();
				break;

			case SPEAKER_PAUSE :
				speakerPause();
				break;

			case SPEAKER_CLEAR :
				speakerClear();
				break;

			case SPEAKER_ADD_NOTE :
				speakerAddNote(reg.speaker.note);
				break;

			case SPEAKER_GET_BUFFER_FREE :
				reg.speaker.bufferFree = speakerGetBufferFree();
				break;
		}
	}
}

/**
 * Select a status register
 *
 * @param dataInRaw Status register address
 */
void ioWriteStatusAddress(unsigned char dataInRaw)
{
	reg.statusAddress = dataInRaw;
}

/**
 * Select a field register
 *
 * @param dataInRaw Field register address
 */
void ioWriteFieldAddress(unsigned char dataInRaw)
{
	reg.fieldAddress = dataInRaw;
}

/**
 * Write data to a field register
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void ioWriteField(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		switch(reg.fieldAddress)
		{
			case RTC_YEAR :
				reg.rtc.year = data;
				break;

			case RTC_MONTH :
				reg.rtc.month = data;
				break;

			case RTC_DAY :
				reg.rtc.day = data;
				break;

			case RTC_HOUR :
				reg.rtc.hour = data;
				break;

			case RTC_MINUTE :
				reg.rtc.minute = data;
				break;

			case RTC_SECOND :
				reg.rtc.second = data;
				break;

			case SPEAKER_NOTE :
				reg.speaker.note.note = data;
				break;

			case SPEAKER_TIME :
				reg.speaker.note.time = data;
				break;

			case SPEAKER_FILL :
				reg.speaker.note.fill = data;
				break;

			case SPEAKER_VOLUME :
				reg.speaker.note.volume = data;
				break;
		}
	}
}

/**
 * Set a color of a character and background
 *
 * @param half Half of register selector
 * @param dataInRaw Data to write
 */
void ioWriteLCDColor(bool half, unsigned char dataInRaw)
{
	if(half == HALF_LOW)
	{
		lcdSetColorForeground(dataInRaw);
	}
	else
	{
		lcdSetColorBackground(dataInRaw);
	}
}

/**
 * Set horizontal cursor position
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void ioWriteLCDCursorPosX(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		lcdSetCursorPosX(data);
	}
}

/**
 * Set vertical cursor position
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void ioWriteLCDCursorPosY(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		lcdSetCursorPosY(data);
	}
}

/**
 * Print a character on the LCD
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void ioWriteLCDChar(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		lcdPrintChar(data);
	}
}

/**
 * Send a byte over RS232
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void ioWriteCRS232Tx(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		rs232Send(data);
	}
}

/**
 * Set a next character of a file name
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void ioWriteFSName(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		if(reg.fs.nameLength < FS_NAME_MAX_LENGTH)
		{
			if((data >= FS_MIN_CODE) && (data <= FS_MAX_CODE))
			{
				reg.fs.name[reg.fs.nameLength] = data;
				reg.fs.nameLength++;
				reg.fs.name[reg.fs.nameLength] = 0x00;
			}
		}
	}
}

/**
 * Create a path to file
 *
 * @param buffer Buffer to store the path
 * @param dirName Directory name where the file is
 * @param fileName Name of the file
 */
static void ioCreatePath(char *buffer, const char *dirName, const char *fileName)
{
	int i = 0;
	
	for(int j = 0; dirName[j] != 0x00; j++, i++)
	{
		buffer[i] = dirName[j];
	}
	
	buffer[i] = '/';
	i++;
	
	for(int j = 0; fileName[j] != 0x00; j++, i++)
	{
		buffer[i] = fileName[j];
	}
	
	buffer[i] = 0x00;
}

//! Process the IRQ event for the IO. It processes a new incoming request from the XiPU.
void EXTI4_15_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET) 
	{
		unsigned char value = ((GPIOB->IDR >> 8) & 0xff);

		bool half = ((value & (1 << 4)) ? HALF_HIGH : HALF_LOW);
		bool mode = ((value & (1 << 5)) ? MODE_DATA_TRANSFER : MODE_REG_SELECT);
		bool rw = ((value & (1 << 6)) ? RW_WRITE : RW_READ);

		if(mode == MODE_REG_SELECT)
		{
			// Select register to work
			regSelected = (value & IO_REG_SELECT_MASK);
		}
		else
		{
			if(rw == RW_READ)
			{
				unsigned char dataOut = 0;

				switch(regSelected)
				{
					case REG_STATUS :
						dataOut = ioReadStatus(half);
						break;

					case REG_FIELD :
						dataOut = ioReadField();
						break;

					case REG_KEYBOARD :
						dataOut = ioReadKeyboard(half);
						break;

					case REG_LCD_CURSOR_POS_X :
						dataOut = ioReadLCDCursorPosX(half);
						break;

					case REG_LCD_CURSOR_POS_Y :
						dataOut = ioReadLCDCursorPosY(half);
						break;

					case REG_RS232 :
						dataOut = ioReadRS232Rx(half);
						break;

					case REG_FS :
						dataOut = ioReadFSData(half);
						break;
				}

				if(half == HALF_HIGH)
				{
					dataOut >>= 4;
				}

				// Register value to out
				GPIOB->ODR = ((GPIOB->ODR & IO_IN_CONTROL_MASK) | (dataOut & IO_IN_DATA_MASK));
			}
			else
			{
				unsigned char dataInRaw = (value & IO_IN_DATA_MASK);
				unsigned char dataIn = dataInRaw;
				unsigned char mask = IO_HALF_UP_DATA_MASK;

				if(half == HALF_HIGH)
				{
					dataIn <<= IO_HALF_DATA_OFFSET;
					mask >>= IO_HALF_DATA_OFFSET;
				}

				switch(regSelected)
				{
					case REG_COMMAND :
						ioWriteCommand(half, dataInRaw);
						break;

					case REG_STATUS :
						ioWriteStatusAddress(dataInRaw);
						break;

					case REG_FIELD_ADDRESS :
						ioWriteFieldAddress(dataInRaw);
						break;

					case REG_FIELD :
						ioWriteField(half, dataIn, mask);
						break;

					case REG_LCD_COLOR :
						ioWriteLCDColor(half, dataInRaw);
						break;

					case REG_LCD_CURSOR_POS_X :
						ioWriteLCDCursorPosX(half, dataIn, mask);
						break;

					case REG_LCD_CURSOR_POS_Y :
						ioWriteLCDCursorPosY(half, dataIn, mask);
						break;

					case REG_LCD_CHAR :
						ioWriteLCDChar(half, dataIn, mask);
						break;

					case REG_RS232 :
						ioWriteCRS232Tx(half, dataIn, mask);
						break;

					case REG_FS :
						ioWriteFSName(half, dataIn, mask);
						break;
				}
			}
		}
		
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
		
		// Operation is completed
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
	}
}
