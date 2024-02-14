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

#include "emu.h"
#include "ui_emu.h"

/**
 * Constructor for the emulator class
 *
 * @param parent Parent object
 */
Emu::Emu(QWidget *parent) : QMainWindow(parent), ui(new Ui::Emu)
{
	ui->setupUi(this);

	this->loaded = false;
	this->started = false;
	this->running = false;

	this->ui->fileUrom0PathLabel->setText("");
	this->ui->fileUrom1PathLabel->setText("");
	this->ui->fileBiosPathLabel->setText("");
	this->ui->fileFSDirPathLabel->setText("");

	this->reset();
	this->update();

	this->ui->speakerVolumeSlider->setValue(50);
	this->ui->speakerVolumeValueLabel->setText("50%");

	this->setFocus();

	QObject::connect(&this->cpu, SIGNAL(updateSignal()), this, SLOT(updateSlot()));

	QObject::connect(&this->io, SIGNAL(updateLCDBufferSignal(LCD::Buffer)), ui->lcdBufferView, SLOT(drawSlot(LCD::Buffer)));

	QObject::connect(&this->io, SIGNAL(updateLEDRunSignal(bool)), this, SLOT(updateLEDRunSlot(bool)));
	QObject::connect(&this->io, SIGNAL(updateLEDErrorSignal(bool)), this, SLOT(updateLEDErrorSlot(bool)));

	QObject::connect(&this->io, SIGNAL(updateRS232TxSignal(unsigned char)), this, SLOT(updateRS232TxSlot(unsigned char)));
	QObject::connect(&this->io, SIGNAL(updateRS232RxSignal(unsigned char)), this, SLOT(updateRS232RxSlot(unsigned char)));

	QObject::connect(&this->io, SIGNAL(updateRTCDateTimeSignal(QDateTime)), this, SLOT(updateRTCDateTimeSlot(QDateTime)));

	QObject::connect(&this->io, SIGNAL(updateSpeakerStatusSignal(int)), this, SLOT(updateSpeakerStatusSlot(int)));

	QObject::connect(&this->cpu, SIGNAL(outSignal(unsigned char)), &this->io, SLOT(outSlot(unsigned char)));
	QObject::connect(&this->io, SIGNAL(inSignal(unsigned char)), &this->cpu, SLOT(inSlot(unsigned char)));
}

//! Destructor for the emulator class
Emu::~Emu()
{
	QObject::disconnect(&this->io);
	QObject::disconnect(&this->cpu);

	QObject::disconnect(this);

	delete ui;
}

//! Reset the UI elements
void Emu::reset()
{
	this->ui->regAValueHexLabel->setText("");
	this->ui->regAValueDecLabel->setText("");
	this->ui->regAValueBinLabel->setText("");

	this->ui->regBValueHexLabel->setText("");
	this->ui->regBValueDecLabel->setText("");
	this->ui->regBValueBinLabel->setText("");

	this->ui->regXValueHexLabel->setText("");
	this->ui->regXValueDecLabel->setText("");
	this->ui->regXValueBinLabel->setText("");

	this->ui->regYValueHexLabel->setText("");
	this->ui->regYValueDecLabel->setText("");
	this->ui->regYValueBinLabel->setText("");

	this->ui->regINValueHexLabel->setText("");
	this->ui->regINValueDecLabel->setText("");
	this->ui->regINValueBinLabel->setText("");

	this->ui->regOUTValueHexLabel->setText("");
	this->ui->regOUTValueDecLabel->setText("");
	this->ui->regOUTValueBinLabel->setText("");

	this->ui->regDValueHexLabel->setText("");
	this->ui->regDValueDecLabel->setText("");
	this->ui->regDValueBinLabel->setText("");

	this->ui->regTValueHexLabel->setText("");
	this->ui->regTValueDecLabel->setText("");
	this->ui->regTValueBinLabel->setText("");

	this->ui->regIValueHexLabel->setText("");
	this->ui->regIValueBinLabel->setText("");

	this->ui->regPCHValueLabel->setText("");
	this->ui->regPCLValueLabel->setText("");

	this->ui->regCValueLabel->setText("");
	this->ui->regZValueLabel->setText("");

	this->ui->regSPHValueLabel->setText("");
	this->ui->regSPLValueLabel->setText("");
	this->ui->regSPMaxValueLabel->setText("");

	this->ui->regBPHValueLabel->setText("");
	this->ui->regBPLValueLabel->setText("");

	this->ui->regMAHValueLabel->setText("");
	this->ui->regMALValueLabel->setText("");

	this->ui->emuTimeValueLabel->setText("");

	this->ui->lcdBufferView->reset();

	this->ui->ledRunValueLabel->setText("");
	this->ui->ledRunValueLabel->setStyleSheet("");

	this->ui->ledErrorValueLabel->setText("");
	this->ui->ledErrorValueLabel->setStyleSheet("");

	this->ramSetPage(0);

	this->ui->rs232RxText->clear();
	this->ui->rs232TxText->clear();
	this->ui->rs232RxEdit->clear();

	QDateTime dateTime;
	dateTime.setDate(QDate(2000, 1, 1));

	this->ui->rtcValueLabel->setText(dateTime.toString("yyyy.MM.dd hh:mm:ss"));

	this->ui->speakerBufferValueLabel->setText("0");
}

//! Update internal statuses and UI elements
void Emu::update()
{
	if(!this->started)
	{
		if((!this->ui->fileUrom0PathLabel->text().isEmpty()) && (!this->ui->fileUrom1PathLabel->text().isEmpty()) && (!this->ui->fileBiosPathLabel->text().isEmpty()) && (!this->ui->fileFSDirPathLabel->text().isEmpty()))
		{
			this->loaded = true;
		}
		else
		{
			this->loaded = false;
		}
	}

	this->ui->fileUrom0OpenButton->setEnabled(!this->started);
	this->ui->fileUrom1OpenButton->setEnabled(!this->started);
	this->ui->fileBiosOpenButton->setEnabled(!this->started);
	this->ui->fileFSDirOpenButton->setEnabled(!this->started);

	this->ui->fileUrom0ReloadButton->setEnabled((!this->started) && (!this->ui->fileUrom0PathLabel->text().isEmpty()));
	this->ui->fileUrom1ReloadButton->setEnabled((!this->started) && (!this->ui->fileUrom1PathLabel->text().isEmpty()));
	this->ui->fileBiosReloadButton->setEnabled((!this->started) && (!this->ui->fileBiosPathLabel->text().isEmpty()));

	if(this->loaded)
	{
		if(this->running)
		{
			this->ui->emuStatusValueLabel->setText("Running");
		}
		else
		{
			if(this->started)
			{
				this->ui->emuStatusValueLabel->setText("Paused");
			}
			else
			{
				this->ui->emuStatusValueLabel->setText("Stopped");
			}
		}
	}
	else
	{
		this->ui->emuStatusValueLabel->setText("");
	}

	this->ui->emuControlRunButton->setEnabled(this->loaded && (!this->running));
	this->ui->emuControlStepButton->setEnabled(this->loaded && (!this->running));
	this->ui->emuControlPauseButton->setEnabled(this->running);
	this->ui->emuControlStopButton->setEnabled(this->started);

	this->ui->ramGoToBIOSButton->setEnabled(this->started);
	this->ui->ramGoToOSButton->setEnabled(this->started);
	this->ui->ramGoToAppButton->setEnabled(this->started);
	this->ui->ramGoToSPButton->setEnabled(this->started);

	this->ui->rs232RxSendButton->setEnabled(this->started);
	this->ui->rtcSetCurrentButton->setEnabled(this->started);
	this->ui->speakerVolumeSlider->setEnabled(this->started);
}

/**
 * Set RAM page to show
 *
 * @param page Page number
 */
void Emu::ramSetPage(int page)
{
	if((page >= 0) && ((CPU::MEMORY_SIZE / CPU::MEMORY_PAGE_SIZE) > page))
	{
		this->ramPage = page;

		this->ui->ramText->clear();

		if(this->started)
		{
			const QVector<unsigned char> *memory;

			if((page * CPU::MEMORY_PAGE_SIZE) < CPU::BIOS_SIZE)
			{
				memory = &(this->cpu.getBios().data);
			}
			else
			{
				memory = &(this->cpu.getRam().data);
			}

			QString text = "";

			for(int i = 0; i < CPU::MEMORY_PAGE_SIZE; i++)
			{
				if(((i % 16) == 0) && (i != 0))
				{
					text.append("\n");
				}
				else if((i % 16) != 0)
				{
					text.append(" ");
				}

				text.append(QString("%1").arg(memory->at((this->ramPage * CPU::MEMORY_PAGE_SIZE) + i), 2, 16, QChar('0')));
			}

			this->ui->ramText->setPlainText(text);

			this->ui->ramMinusButton->setEnabled(page != 0);
			this->ui->ramPlusButton->setEnabled(page < ((CPU::MEMORY_SIZE / CPU::MEMORY_PAGE_SIZE) - 1));
		}
		else
		{
			this->ui->ramMinusButton->setEnabled(false);
			this->ui->ramPlusButton->setEnabled(false);
		}
	}

	QString addressPrefix = QString("%1").arg(this->ramPage, 2, 16, QChar('0')).toUpper();
	QString address = "";

	address.append(addressPrefix);
	address.append("00 - ");
	address.append(addressPrefix);
	address.append("FF");

	this->ui->ramAddressLabel->setText(QString().append(address));
}

/**
 * Load data from file to the uROM buffer
 *
 * @param path Path to the file to load
 * @param urom Buffer of the uROM to fill
 *
 * @return Status of loading data to the memory buffer
 */
bool Emu::loadUromFile(const QString &path, CPU::UROM &urom)
{
	QFile file(path);

	if(!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, "Error", "Unable to open file");
		return(false);
	}

	if(CPU::UROM_SIZE != file.read(reinterpret_cast<char *>(urom.data.data()), CPU::UROM_SIZE))
	{
		QMessageBox::critical(this, "Error", "Bad size of file");
		return(false);
	}

	file.close();

	return(true);
}

/**
 * Load data from file to the BIOS buffer
 *
 * @param path Path to the file to load
 * @param bios Buffer of the BIOS to fill
 *
 * @return Status of loading data to the memory buffer
 */
bool Emu::loadBiosFile(const QString &path, CPU::BIOS &bios)
{
	QFile file(path);

	if(!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, "Error", "Unable to open file");
		return(false);
	}

	if(CPU::BIOS_SIZE != file.read(reinterpret_cast<char *>(bios.data.data()), CPU::BIOS_SIZE))
	{
		QMessageBox::critical(this, "Error", "Bad size of file");
		return(false);
	}

	file.close();

	return(true);
}

/**
 * Process the mouse press event
 *
 * @param event Mouse event
 */
void Emu::mousePressEvent(QMouseEvent *)
{
	this->setFocus();
}

/**
 * Process the focus selector event
 *
 * @param next Search direction of the focus
 *
 * @return Status of finding next/previous widget to give the keyboard focus to
 */
bool Emu::focusNextPrevChild(bool)
{
	return(false);
}

/**
 * Process the key press event
 *
 * @param event Key event
 */
void Emu::keyPressEvent(QKeyEvent *event)
{
	if(this->running)
	{
		this->io.keyboardKeyPress(event->key(), event->modifiers());
	}
}

//! Update time, registers, memory view UI elements
void Emu::updateSlot()
{
	unsigned long long ticks = this->cpu.getTicks();

	QString timeMs = QString("%1").arg(((ticks / (CPU::FREQUENCY / 1000)) % 1000), 3, 10, QChar('0'));
	QString timeSec = QString("%1").arg(((ticks / CPU::FREQUENCY) % 60), 2, 10, QChar('0'));
	QString timeMin = QString("%1").arg(((ticks / (60 * CPU::FREQUENCY)) % 60), 2, 10, QChar('0'));
	QString timeHour = QString("%1").arg(((ticks / (60 * CPU::FREQUENCY) / 60)), 2, 10, QChar('0'));

	this->ui->emuTimeValueLabel->setText(QString("%1:%2:%3.%4").arg(timeHour, timeMin, timeSec, timeMs));

	const CPU::Reg &reg = this->cpu.getReg();

	this->ui->regAValueHexLabel->setText(QString("%1").arg(reg.a, 2, 16, QChar('0')));
	this->ui->regAValueDecLabel->setText(QString("%1").arg(reg.a));
	this->ui->regAValueBinLabel->setText(QString("%1").arg(reg.a, 8, 2, QChar('0')));

	this->ui->regBValueHexLabel->setText(QString("%1").arg(reg.b, 2, 16, QChar('0')));
	this->ui->regBValueDecLabel->setText(QString("%1").arg(reg.b));
	this->ui->regBValueBinLabel->setText(QString("%1").arg(reg.b, 8, 2, QChar('0')));

	this->ui->regXValueHexLabel->setText(QString("%1").arg(reg.x, 2, 16, QChar('0')));
	this->ui->regXValueDecLabel->setText(QString("%1").arg(reg.x));
	this->ui->regXValueBinLabel->setText(QString("%1").arg(reg.x, 8, 2, QChar('0')));

	this->ui->regYValueHexLabel->setText(QString("%1").arg(reg.y, 2, 16, QChar('0')));
	this->ui->regYValueDecLabel->setText(QString("%1").arg(reg.y));
	this->ui->regYValueBinLabel->setText(QString("%1").arg(reg.y, 8, 2, QChar('0')));

	this->ui->regINValueHexLabel->setText(QString("%1").arg(reg.in, 2, 16, QChar('0')));
	this->ui->regINValueDecLabel->setText(QString("%1").arg(reg.in));
	this->ui->regINValueBinLabel->setText(QString("%1").arg(reg.in, 8, 2, QChar('0')));

	this->ui->regOUTValueHexLabel->setText(QString("%1").arg(reg.out, 2, 16, QChar('0')));
	this->ui->regOUTValueDecLabel->setText(QString("%1").arg(reg.out));
	this->ui->regOUTValueBinLabel->setText(QString("%1").arg(reg.out, 8, 2, QChar('0')));

	this->ui->regDValueHexLabel->setText(QString("%1").arg(reg.d, 2, 16, QChar('0')));
	this->ui->regDValueDecLabel->setText(QString("%1").arg(reg.d));
	this->ui->regDValueBinLabel->setText(QString("%1").arg(reg.d, 8, 2, QChar('0')));

	this->ui->regTValueHexLabel->setText(QString("%1").arg(reg.t, 2, 16, QChar('0')));
	this->ui->regTValueDecLabel->setText(QString("%1").arg(reg.t));
	this->ui->regTValueBinLabel->setText(QString("%1").arg(reg.t, 8, 2, QChar('0')));

	this->ui->regIValueHexLabel->setText(QString("%1").arg(reg.i, 2, 16, QChar('0')));
	this->ui->regIValueBinLabel->setText(QString("%1").arg(reg.i, 8, 2, QChar('0')));

	this->ui->regPCHValueLabel->setText(QString("%1").arg(reg.pch, 2, 16, QChar('0')));
	this->ui->regPCLValueLabel->setText(QString("%1").arg(reg.pcl, 2, 16, QChar('0')));

	this->ui->regCValueLabel->setText(QString("%1%2").arg(static_cast<int>(reg.c[1])).arg(static_cast<int>(reg.c[0])));
	this->ui->regZValueLabel->setText(QString("%1%2").arg(static_cast<int>(reg.z[1])).arg(static_cast<int>(reg.z[0])));

	this->ui->regSPHValueLabel->setText(QString("%1").arg(reg.sph, 2, 16, QChar('0')));
	this->ui->regSPLValueLabel->setText(QString("%1").arg(reg.spl, 2, 16, QChar('0')));
	this->ui->regSPMaxValueLabel->setText(QString("%1").arg(reg.maxSp, 4, 16, QChar('0')));

	this->ui->regBPHValueLabel->setText(QString("%1").arg(reg.bph, 2, 16, QChar('0')));
	this->ui->regBPLValueLabel->setText(QString("%1").arg(reg.bpl, 2, 16, QChar('0')));

	this->ui->regMAHValueLabel->setText(QString("%1").arg(reg.mah, 2, 16, QChar('0')));
	this->ui->regMALValueLabel->setText(QString("%1").arg(reg.mal, 2, 16, QChar('0')));

	this->ramSetPage(this->ramPage);
}

/**
 * Update status of run LED
 *
 * @param enable LED enable
 */
void Emu::updateLEDRunSlot(bool enable)
{
	this->ui->ledRunValueLabel->setText(enable ? "ON" : "");
	this->ui->ledRunValueLabel->setStyleSheet(enable ? "QLabel { color : blue; }" : "");
}

/**
 * Update status of error LED
 *
 * @param enable LED enable
 */
void Emu::updateLEDErrorSlot(bool enable)
{
	this->ui->ledErrorValueLabel->setText(enable ? "ON" : "");
	this->ui->ledErrorValueLabel->setStyleSheet(enable ? "QLabel { color : red; }" : "");
}

/**
 * Update status of transmited data via RS232
 *
 * @param c Transmited char
 */
void Emu::updateRS232TxSlot(unsigned char c)
{
	this->ui->rs232TxText->moveCursor(QTextCursor::End);

	if(c != 0)
	{
		this->ui->rs232TxText->insertPlainText(QString(c));
	}
	else
	{
		this->ui->rs232TxText->insertPlainText("\n");
	}

	QScrollBar *scrollBar = this->ui->rs232TxText->verticalScrollBar();

	scrollBar->setValue(scrollBar->maximum());
}

/**
 * Update status of received data via RS232
 *
 * @param c Received char
 */
void Emu::updateRS232RxSlot(unsigned char c)
{
	this->ui->rs232RxText->moveCursor(QTextCursor::End);

	if(c != 0)
	{
		this->ui->rs232RxText->insertPlainText(QString(c));
	}
	else
	{
		this->ui->rs232RxText->insertPlainText("\n");
	}

	QScrollBar *scrollBar = this->ui->rs232RxText->verticalScrollBar();

	scrollBar->setValue(scrollBar->maximum());
}

/**
 * Update status of date and time
 *
 * @param dateTime Date and time
 */
void Emu::updateRTCDateTimeSlot(const QDateTime &dateTime)
{
	this->ui->rtcValueLabel->setText(dateTime.toString("yyyy.MM.dd hh:mm:ss"));
}

/**
 * Update status of the speaker buffer usage
 *
 * @param bufferUsed Buffer usage
 */
void Emu::updateSpeakerStatusSlot(int bufferUsed)
{
	this->ui->speakerBufferValueLabel->setText(QString("%1").arg(bufferUsed));
}

//! Process first uROM open event
void Emu::on_fileUrom0OpenButton_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, "", "", "UROM 0 (*.bin)");

	if(!path.isEmpty())
	{
		CPU::UROM urom0;

		if(this->loadUromFile(path, urom0))
		{
			this->ui->fileUrom0PathLabel->setText(path);

			this->cpu.setUrom0(urom0);
		}
		else
		{
			this->ui->fileUrom0PathLabel->setText("");
		}

		this->update();
	}
}

//! Process second uROM open event
void Emu::on_fileUrom1OpenButton_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, "", "", "UROM 1 (*.bin)");

	if(!path.isEmpty())
	{
		CPU::UROM urom1;

		if(this->loadUromFile(path, urom1))
		{
			this->ui->fileUrom1PathLabel->setText(path);

			this->cpu.setUrom1(urom1);
		}
		else
		{
			this->ui->fileUrom1PathLabel->setText("");
		}

		this->update();
	}
}

//! Process ROM open event
void Emu::on_fileBiosOpenButton_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, "", "", "BIOS (*.bin)");

	if(!path.isEmpty())
	{
		CPU::BIOS bios;

		if(this->loadBiosFile(path, bios))
		{
			this->ui->fileBiosPathLabel->setText(path);

			this->cpu.setBios(bios);
		}
		else
		{
			this->ui->fileBiosPathLabel->setText("");
		}

		this->update();
	}
}

//! Process file system open event
void Emu::on_fileFSDirOpenButton_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, "", "");

	if(!path.isEmpty())
	{
		this->ui->fileFSDirPathLabel->setText(path);

		this->io.fsSetPath(path);

		this->update();
	}
}

//! Process first uROM reload event
void Emu::on_fileUrom0ReloadButton_clicked()
{
	CPU::UROM urom0;

	if(this->loadUromFile(this->ui->fileUrom0PathLabel->text(), urom0))
	{
		this->cpu.setUrom0(urom0);
	}
	else
	{
		this->ui->fileUrom0PathLabel->setText("");
	}

	this->update();
}

//! Process second uROM reload event
void Emu::on_fileUrom1ReloadButton_clicked()
{
	CPU::UROM urom1;

	if(this->loadUromFile(this->ui->fileUrom1PathLabel->text(), urom1))
	{
		this->cpu.setUrom1(urom1);
	}
	else
	{
		this->ui->fileUrom1PathLabel->setText("");
	}

	this->update();
}

//! Process ROM reload event
void Emu::on_fileBiosReloadButton_clicked()
{
	CPU::BIOS bios;

	if(this->loadBiosFile(this->ui->fileBiosPathLabel->text(), bios))
	{
		this->cpu.setBios(bios);
	}
	else
	{
		this->ui->fileBiosPathLabel->setText("");
	}

	this->update();
}

//! Process run emulation event
void Emu::on_emuControlRunButton_clicked()
{
	this->started = true;
	this->running = true;

	this->update();

	this->cpu.run();
}

//! Process a make step emulation event
void Emu::on_emuControlStepButton_clicked()
{
	this->started = true;

	this->update();

	this->cpu.step();
}

//! Process pause emulation event
void Emu::on_emuControlPauseButton_clicked()
{
	this->running = false;

	this->update();

	this->cpu.pause();
}

//! Process stop emulation event
void Emu::on_emuControlStopButton_clicked()
{
	this->started = false;
	this->running = false;

	this->cpu.stop();
	this->io.reset();

	this->reset();
	this->update();
}

/**
 * Process set volume level event
 *
 * @param value Volume level
 */
void Emu::on_speakerVolumeSlider_valueChanged(int value)
{
	this->io.speakerSetVolume(static_cast<unsigned int>(value));

	this->ui->speakerVolumeValueLabel->setText(QString("%1%").arg(value));
}

//! Process set current time event
void Emu::on_rtcSetCurrentButton_clicked()
{
	QDateTime dateTime = QDateTime::currentDateTime();

	this->io.rtcSetDateTime(dateTime);
}

//! Process go to the BIOS section for RAM event
void Emu::on_ramGoToBIOSButton_clicked()
{
	this->ramSetPage(CPU::MEMORY_BIOS_ADDRESS / CPU::MEMORY_PAGE_SIZE);
}

//! Process go to the OS section for RAM event
void Emu::on_ramGoToOSButton_clicked()
{
	this->ramSetPage(CPU::MEMORY_OS_ADDRESS / CPU::MEMORY_PAGE_SIZE);
}

//! Process go to the application section for RAM event
void Emu::on_ramGoToAppButton_clicked()
{
	this->ramSetPage(CPU::MEMORY_APP_ADDRESS / CPU::MEMORY_PAGE_SIZE);
}

//! Process go to the Stack section for RAM event
void Emu::on_ramGoToSPButton_clicked()
{
	this->ramSetPage(CPU::MEMORY_SP_ADDRESS / CPU::MEMORY_PAGE_SIZE);
}

//! Process go to the previous page for RAM event
void Emu::on_ramMinusButton_clicked()
{
	this->ramSetPage(this->ramPage - 1);
}

//! Process go to the next page for RAM event
void Emu::on_ramPlusButton_clicked()
{
	this->ramSetPage(this->ramPage + 1);
}

//! Process send the transmit buffer data via RS232 event
void Emu::on_rs232RxSendButton_clicked()
{
	QString text = this->ui->rs232RxEdit->text();

	this->ui->rs232RxEdit->clear();

	this->io.rs232Receive(text);
}
