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

#ifndef IO_H
#define IO_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QCoreApplication>

#include "keyboard.h"
#include "led.h"
#include "lcd.h"
#include "rs232.h"
#include "rtc.h"
#include "speaker.h"
#include "fs.h"

//! This class contains IO functions. It is a motherboard simulation part for the CPU.
class IO : public QObject
{
	Q_OBJECT

	public:
		static const unsigned char IN_DATA_MASK = 0x0f; //!< Input data mask for the data part of the BUS
		static const unsigned char IN_CONTROL_MASK = 0xf0; //!< Input data mask for the control bits part of the BUS

		static const unsigned char IN_CLOCK_BIT = (1 << 4); //!< Input clock control bit
		static const unsigned char IN_KEYBOARD_READY_BIT = (1 << 5); //!< Input "keyboard has data to read" control bit
		static const unsigned char IN_RS232_READY_BIT = (1 << 6); //!< Input "RS232 has data to read" control bit
		static const unsigned char IN_OPERATION_COMPLETE_BIT = (1 << 7); //!< Input operation completed control bit

		static const unsigned char OUT_DATA_MASK = 0x0f; //!< Output data mask for the data part of the BUS
		static const unsigned char OUT_CONTROL_MASK = 0xf0; //!< Output data mask for the control bits part of the BUS

		static const unsigned char OUT_HALF_BIT = (1 << 4); //!< Output "half selector of the byte" control bit
		static const unsigned char OUT_MODE_BIT = (1 << 5); //!< Output "register selection or data transision mode" control bit
		static const unsigned char OUT_RW_BIT = (1 << 6); //!< Output read/write control bit
		static const unsigned char OUT_DATA_READY_BIT = (1 << 7); //!< Output "data is ready to read from output" control bit

		static const unsigned char REG_SELECT_MASK = 0x0f; //!< Register selection mask

		static const unsigned char HALF_UP_DATA_MASK = 0xf0; //!< Upper half of the byte mask
		static const int HALF_DATA_OFFSET = 4; //!< Upper half of the byte offset

		IO(QObject *parent = nullptr);
		~IO() override;

		IO(const IO &) = delete;
		IO &operator=(const IO &) = delete;
		IO(IO &&) = delete;
		IO &operator=(IO &&) = delete;

		void reset();

		void keyboardKeyPress(int key, Qt::KeyboardModifiers modifiers);
		void ledSetRun(bool enable);
		void ledSetError(bool enable);
		void rs232Receive(const QString &text);
		void rtcSetDateTime(const QDateTime &dateTime);
		void speakerSetVolume(unsigned int volume);
		void fsSetPath(const QString &path);

	private:
		static const bool HALF_LOW = false; //!< Low half of data transfer
		static const bool HALF_HIGH = true; //!< High half of data transfer

		static const bool MODE_REG_SELECT = false; //!< Register select mode
		static const bool MODE_DATA_TRANSFER = true; //!< Data transfer mode

		static const bool RW_READ = false; //!< Read data mode
		static const bool RW_WRITE = true; //!< Write data mode

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

		unsigned char regSelected = 0; //!< Current selected register

		//!< This struct contains motherboard's virtual registers buffer
		struct
		{
			unsigned char statusAddress; //!< Select a status register
			unsigned char fieldAddress; //!< Select a field register
			RTC::DateTime rtc; //!< RTC data buffer
			struct
			{
				Speaker::Note note; //!< Note parameters buffer
				unsigned char bufferFree; //!< Free space size in the sound buffer
			} speaker; //!< Speaker data buffer
			struct
			{
				QString name; //!< Path buffer
				FS::Size size; //!< Size buffer
				bool status; //!< Status buffer
			} fs; //!< File System buffer
		} reg = {};

		volatile unsigned char in = 0; //!< Input register buffer

		Keyboard keyboard; //!< Keyboard class instance used for emulation motherboard's IO part
		LED led; //!< LED class instance used for emulation motherboard's IO part
		LCD lcd; //!< LCD class instance used for emulation motherboard's IO part
		RS232 rs232; //!< RS232 class instance used for emulation motherboard's IO part
		RTC rtc; //!< Real Time Clock class instance used for emulation motherboard's IO part
		Speaker speaker; //!< Speaker class instance used for emulation motherboard's IO part
		FS fs; //!< File System class instance used for emulation motherboard's IO part

		unsigned char outReadStatus(bool half);
		unsigned char outReadField();
		unsigned char outReadKeyboard(bool half);
		unsigned char outReadLCDCursorPosX(bool half);
		unsigned char outReadLCDCursorPosY(bool half);
		unsigned char outReadRS232Rx(bool half);
		unsigned char outReadFSData(bool half);

		void outWriteCommand(bool half, unsigned char dataInRaw);
		void outWriteStatusAddress(unsigned char dataInRaw);
		void outWriteFieldAddress(unsigned char dataInRaw);
		void outWriteField(bool half, unsigned char dataIn, unsigned char mask);
		void outWriteLCDColor(bool half, unsigned char dataInRaw);
		void outWriteLCDCursorPosX(bool half, unsigned char dataIn, unsigned char mask);
		void outWriteLCDCursorPosY(bool half, unsigned char dataIn, unsigned char mask);
		void outWriteLCDChar(bool half, unsigned char dataIn, unsigned char mask);
		void outWriteCRS232Tx(bool half, unsigned char dataIn, unsigned char mask);
		void outWriteFSName(bool half, unsigned char dataIn, unsigned char mask);

	signals:
		void updateLEDRunSignal(bool enable);
		void updateLEDErrorSignal(bool enable);

		void updateLCDBufferSignal(LCD::Buffer buffer);

		void updateRS232TxSignal(unsigned char c);
		void updateRS232RxSignal(unsigned char c);

		void updateRTCDateTimeSignal(QDateTime dateTime);

		void updateSpeakerStatusSignal(int bufferUsed);

		void inSignal(unsigned char in);

	public slots:
		void keyboardDataReadySlot(bool ready);
		void rs232DataReadySlot(bool ready);

		void outSlot(unsigned char out);
};

#endif
