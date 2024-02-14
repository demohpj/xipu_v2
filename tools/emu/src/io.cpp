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

#include "io.h"

/**
 * Constructor for the IO class. It connects all communication classes for IO operations.
 *
 * @param parent Parent object
 */
IO::IO(QObject *parent) : QObject(parent)
{
	this->reset();

	QObject::connect(&this->led, SIGNAL(updateRunSignal(bool)), this, SIGNAL(updateLEDRunSignal(bool)));
	QObject::connect(&this->led, SIGNAL(updateErrorSignal(bool)), this, SIGNAL(updateLEDErrorSignal(bool)));

	QObject::connect(&this->lcd, SIGNAL(updateBufferSignal(LCD::Buffer)), this, SIGNAL(updateLCDBufferSignal(LCD::Buffer)));

	QObject::connect(&this->rs232, SIGNAL(updateTxSignal(unsigned char)), this, SIGNAL(updateRS232TxSignal(unsigned char)));
	QObject::connect(&this->rs232, SIGNAL(updateRxSignal(unsigned char)), this, SIGNAL(updateRS232RxSignal(unsigned char)));

	QObject::connect(&this->rtc, SIGNAL(updateDateTimeSignal(QDateTime)), this, SIGNAL(updateRTCDateTimeSignal(QDateTime)));

	QObject::connect(&this->speaker, SIGNAL(updateStatusSignal(int)), this, SIGNAL(updateSpeakerStatusSignal(int)));

	QObject::connect(&this->keyboard, SIGNAL(dataReadySignal(bool)), this, SLOT(keyboardDataReadySlot(bool)));
	QObject::connect(&this->rs232, SIGNAL(dataReadySignal(bool)), this, SLOT(rs232DataReadySlot(bool)));
}

//! Destructor for the IO class
IO::~IO()
{
	QObject::disconnect(this);
}

//! Reset the IO buffers and trigger reset for all connected communication classes
void IO::reset()
{
	this->regSelected = 0;
	this->reg = {};
	this->in = 0;

	this->keyboard.reset();
	this->led.reset();
	this->lcd.reset();
	this->rs232.reset();
	this->rtc.reset();
	this->speaker.reset();
	this->fs.reset();
}

/**
 * Process a key press event
 *
 * @param key Pressed key code
 * @param modifiers Optional modifiers to pressed key
 */
void IO::keyboardKeyPress(int key, Qt::KeyboardModifiers modifiers)
{
	this->keyboard.keyPress(key, modifiers);
}

/**
 * Set enable status of the run LED
 *
 * @param enable Enable status
 */
void IO::ledSetRun(bool enable)
{
	this->led.setRun(enable);
}

/**
 * Set enable status of the error LED
 *
 * @param enable Enable status
 */
void IO::ledSetError(bool enable)
{
	this->led.setError(enable);
}

/**
 * Process a received text event
 *
 * @param text Received text
 */
void IO::rs232Receive(const QString &text)
{
	this->rs232.rx(text);
}

/**
 * Set date and time
 *
 * @param dateTime Date and time to set in own RTC format
 */
void IO::rtcSetDateTime(const QDateTime &dateTime)
{
	this->rtc.setDateTime(dateTime);
}

/**
 * Set volume
 *
 * @param volume Volume level in range 0-100
 */
void IO::speakerSetVolume(unsigned int volume)
{
	this->speaker.setVolume(volume);
}

/**
 * Set path to the emulated file system on the local disk
 *
 * @param path Path to emulated file system
 */
void IO::fsSetPath(const QString &path)
{
	this->fs.setPath(path);
}

/**
 * Process "keyboard data is ready to get" event
 *
 * @param ready Status of ready data to get
 */
void IO::keyboardDataReadySlot(bool ready)
{
	if(ready)
	{
		this->in |= IN_KEYBOARD_READY_BIT;
	}
	else
	{
		this->in &= ~IN_KEYBOARD_READY_BIT;
	}

	emit inSignal(this->in);
}

/**
 * Process "RS232 receive data is ready to get" event
 *
 * @param ready Status of ready data to get
 */
void IO::rs232DataReadySlot(bool ready)
{
	if(ready)
	{
		this->in |= IN_RS232_READY_BIT;
	}
	else
	{
		this->in &= ~IN_RS232_READY_BIT;
	}

	emit inSignal(this->in);
}

/**
 * Read a value of the selected status register
 *
 * @param half Half of register selector
 *
 * @return Read data
 */
unsigned char IO::outReadStatus(bool half)
{
	unsigned char data = 0;

	if(half == HALF_LOW)
	{
		switch(this->reg.statusAddress)
		{
			case StatusLow::FS_STATUS :
				if(this->reg.fs.status)
				{
					data = 1;
				}
				break;
		}
	}
	else
	{
		switch(this->reg.statusAddress)
		{
			case StatusHigh::LED_RUN_ENABLED :
				if(this->led.getRun())
				{
					data = 1;
				}
				break;

			case StatusHigh::LED_ERROR_ENABLED :
				if(this->led.getError())
				{
					data = 1;
				}
				break;

			case StatusHigh::SPEAKER_IS_PLAYING :
				if(this->speaker.isPlaying())
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
unsigned char IO::outReadField()
{
	unsigned char data = 0;

	switch(this->reg.fieldAddress)
	{
		case FieldR::FS_DATA_SIZE_LOW :
			data = this->reg.fs.size.low;
			break;

		case FieldR::FS_DATA_SIZE_HIGH :
			data = this->reg.fs.size.high;
			break;

		case FieldR::SPEAKER_BUFFER_FREE :
			data = this->reg.speaker.bufferFree;
			break;

		case FieldRW::RTC_YEAR :
			data = this->reg.rtc.year;
			break;

		case FieldRW::RTC_MONTH :
			data = this->reg.rtc.month;
			break;

		case FieldRW::RTC_DAY :
			data = this->reg.rtc.day;
			break;

		case FieldRW::RTC_HOUR :
			data = this->reg.rtc.hour;
			break;

		case FieldRW::RTC_MINUTE :
			data = this->reg.rtc.minute;
			break;

		case FieldRW::RTC_SECOND :
			data = this->reg.rtc.second;
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
unsigned char IO::outReadKeyboard(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = this->keyboard.getKey();
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
unsigned char IO::outReadLCDCursorPosX(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = this->lcd.getCursorPosX();
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
unsigned char IO::outReadLCDCursorPosY(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = this->lcd.getCursorPosY();
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
unsigned char IO::outReadRS232Rx(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = this->rs232.receive();
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
unsigned char IO::outReadFSData(bool half)
{
	static unsigned char data = 0;

	if(half == HALF_LOW)
	{
		data = this->fs.readNextDataByte();
	}

	return(data);
}

/**
 * Execute a command
 *
 * @param half Half of register selector
 * @param dataInRaw Command to execute
 */
void IO::outWriteCommand(bool half, unsigned char dataInRaw)
{
	if(half == HALF_LOW)
	{
		switch(dataInRaw)
		{
			case CommandLow::FS_OPEN_OS :
				this->reg.fs.status = this->fs.open("os.bin", this->reg.fs.size);
				break;

			case CommandLow::FS_RESET_NAME :
				this->reg.fs.name.clear();
				break;

			case CommandLow::FS_OPEN_APP :
				this->reg.fs.status = this->fs.open(QString("app/").append(this->reg.fs.name), this->reg.fs.size);
				break;

			case CommandLow::FS_OPEN_FILE :
				this->reg.fs.status = this->fs.open(QString("file/").append(this->reg.fs.name), this->reg.fs.size);
				break;

			case CommandLow::FS_LIST_APP :
				this->reg.fs.status = this->fs.list("app", this->reg.fs.size);
				break;

			case CommandLow::FS_LIST_FILE :
				this->reg.fs.status = this->fs.list("file", this->reg.fs.size);
				break;
		}
	}
	else
	{
		switch(dataInRaw)
		{
			case CommandHigh::LED_RUN_OFF :
				this->led.setRun(false);
				break;

			case CommandHigh::LED_RUN_ON :
				this->led.setRun(true);
				break;

			case CommandHigh::LED_ERROR_OFF :
				this->led.setError(false);
				break;

			case CommandHigh::LED_ERROR_ON :
				this->led.setError(true);
				break;

			case CommandHigh::LCD_CLEAR :
				this->lcd.clear();
				this->lcd.refresh();
				this->lcd.update();
				break;

			case CommandHigh::LCD_REFRESH :
				this->lcd.refresh();
				this->lcd.update();
				break;

			case CommandHigh::LCD_SCROLL_DOWN :
				this->lcd.scrollDown();
				break;

			case CommandHigh::LCD_NEW_LINE :
				this->lcd.newLine();
				break;

			case CommandHigh::RTC_WRITE :
				this->rtc.setDateTime(this->reg.rtc);
				break;

			case CommandHigh::RTC_READ :
				this->reg.rtc = this->rtc.getDateTime();
				break;

			case CommandHigh::SPEAKER_PLAY :
				this->speaker.play();
				break;

			case CommandHigh::SPEAKER_PAUSE :
				this->speaker.pause();
				break;

			case CommandHigh::SPEAKER_CLEAR :
				this->speaker.clear();
				break;

			case CommandHigh::SPEAKER_ADD_NOTE :
				this->speaker.addNote(this->reg.speaker.note);
				break;

			case CommandHigh::SPEAKER_GET_BUFFER_FREE :
				this->reg.speaker.bufferFree = this->speaker.getBufferFree();
				break;
		}
	}
}

/**
 * Select a status register
 *
 * @param dataInRaw Status register address
 */
void IO::outWriteStatusAddress(unsigned char dataInRaw)
{
	this->reg.statusAddress = dataInRaw;
}

/**
 * Select a field register
 *
 * @param dataInRaw Field register address
 */
void IO::outWriteFieldAddress(unsigned char dataInRaw)
{
	this->reg.fieldAddress = dataInRaw;
}

/**
 * Write data to a field register
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void IO::outWriteField(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		switch(this->reg.fieldAddress)
		{
			case FieldRW::RTC_YEAR :
				this->reg.rtc.year = data;
				break;

			case FieldRW::RTC_MONTH :
				this->reg.rtc.month = data;
				break;

			case FieldRW::RTC_DAY :
				this->reg.rtc.day = data;
				break;

			case FieldRW::RTC_HOUR :
				this->reg.rtc.hour = data;
				break;

			case FieldRW::RTC_MINUTE :
				this->reg.rtc.minute = data;
				break;

			case FieldRW::RTC_SECOND :
				this->reg.rtc.second = data;
				break;

			case FieldW::SPEAKER_NOTE :
				this->reg.speaker.note.note = data;
				break;

			case FieldW::SPEAKER_TIME :
				this->reg.speaker.note.time = data;
				break;

			case FieldW::SPEAKER_FILL :
				this->reg.speaker.note.fill = data;
				break;

			case FieldW::SPEAKER_VOLUME :
				this->reg.speaker.note.volume = data;
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
void IO::outWriteLCDColor(bool half, unsigned char dataInRaw)
{
	if(half == HALF_LOW)
	{
		this->lcd.setColorForeground(dataInRaw);
	}
	else
	{
		this->lcd.setColorBackground(dataInRaw);
	}
}

/**
 * Set horizontal cursor position
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void IO::outWriteLCDCursorPosX(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		this->lcd.setCursorPosX(data);
	}
}

/**
 * Set vertical cursor position
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void IO::outWriteLCDCursorPosY(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		this->lcd.setCursorPosY(data);
	}
}

/**
 * Print a character on the LCD
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void IO::outWriteLCDChar(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		this->lcd.printChar(data);
	}
}

/**
 * Send a byte over RS232
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void IO::outWriteCRS232Tx(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		this->rs232.send(data);
	}
}

/**
 * Set a next character of a file name
 *
 * @param half Half of register selector
 * @param dataIn Data to write
 * @param mask Mask of input data
 */
void IO::outWriteFSName(bool half, unsigned char dataIn, unsigned char mask)
{
	static unsigned char data = 0;

	data = ((data & mask) | dataIn);

	if(half == HALF_HIGH)
	{
		if(this->reg.fs.name.length() < FS::NAME_MAX_LENGTH)
		{
			if((data >= FS::MIN_CODE) && (data <= FS::MAX_CODE))
			{
				this->reg.fs.name.append(data);
			}
		}
	}
}

/**
 * Process "update the output register value" event.
 * Fill up the output register data half with corresponding data from selected virtual register.
 *
 * @param out Output register value with set up data of the flags half
 */
void IO::outSlot(unsigned char out)
{
	if((out & OUT_DATA_READY_BIT) != 0)
	{
		unsigned char value = out;

		bool half = ((value & OUT_HALF_BIT) ? HALF_HIGH : HALF_LOW);
		bool mode = ((value & OUT_MODE_BIT) ? MODE_DATA_TRANSFER : MODE_REG_SELECT);
		bool rw = ((value & OUT_RW_BIT) ? RW_WRITE : RW_READ);

		if(mode == MODE_REG_SELECT)
		{
			// Select register to work
			this->regSelected = (value & REG_SELECT_MASK);
		}
		else
		{
			if(rw == RW_READ)
			{
				unsigned char dataOut = 0;

				switch(this->regSelected)
				{
					case RegAddress::REG_STATUS :
						dataOut = this->outReadStatus(half);
						break;

					case RegAddress::REG_FIELD :
						dataOut = this->outReadField();
						break;

					case RegAddress::REG_KEYBOARD :
						dataOut = this->outReadKeyboard(half);
						break;

					case RegAddress::REG_LCD_CURSOR_POS_X :
						dataOut = this->outReadLCDCursorPosX(half);
						break;

					case RegAddress::REG_LCD_CURSOR_POS_Y :
						dataOut = this->outReadLCDCursorPosY(half);
						break;

					case RegAddress::REG_RS232 :
						dataOut = this->outReadRS232Rx(half);
						break;

					case RegAddress::REG_FS :
						dataOut = this->outReadFSData(half);
						break;
				}

				if(half == HALF_HIGH)
				{
					dataOut >>= 4;
				}

				// Register value to out
				this->in = ((this->in & IN_CONTROL_MASK) | (dataOut & IN_DATA_MASK));
			}
			else
			{
				unsigned char dataInRaw = (value & IN_DATA_MASK);
				unsigned char dataIn = dataInRaw;
				unsigned char mask = HALF_UP_DATA_MASK;

				if(half == HALF_HIGH)
				{
					dataIn <<= HALF_DATA_OFFSET;
					mask >>= HALF_DATA_OFFSET;
				}

				switch(this->regSelected)
				{
					case RegAddress::REG_COMMAND :
						this->outWriteCommand(half, dataInRaw);
						break;

					case RegAddress::REG_STATUS :
						this->outWriteStatusAddress(dataInRaw);
						break;

					case RegAddress::REG_FIELD_ADDRESS :
						this->outWriteFieldAddress(dataInRaw);
						break;

					case RegAddress::REG_FIELD :
						this->outWriteField(half, dataIn, mask);
						break;

					case RegAddress::REG_LCD_COLOR :
						this->outWriteLCDColor(half, dataInRaw);
						break;

					case RegAddress::REG_LCD_CURSOR_POS_X :
						this->outWriteLCDCursorPosX(half, dataIn, mask);
						break;

					case RegAddress::REG_LCD_CURSOR_POS_Y :
						this->outWriteLCDCursorPosY(half, dataIn, mask);
						break;

					case RegAddress::REG_LCD_CHAR :
						this->outWriteLCDChar(half, dataIn, mask);
						break;

					case RegAddress::REG_RS232 :
						this->outWriteCRS232Tx(half, dataIn, mask);
						break;

					case RegAddress::REG_FS :
						this->outWriteFSName(half, dataIn, mask);
						break;
				}
			}
		}

		// Operation is completed
		this->in ^= IN_OPERATION_COMPLETE_BIT;

		emit inSignal(this->in);
	}
}
