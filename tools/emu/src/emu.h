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

#ifndef EMU_H
#define EMU_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScrollBar>

#include "cpu.h"
#include "io.h"

//! User Interface namespace
namespace Ui
{
	class Emu; //!< Form class for the emulator window
}

class Emu : public QMainWindow
{
	Q_OBJECT

	public:
		Emu(QWidget *parent = nullptr);
		~Emu() override;

		Emu(const Emu &) = delete;
		Emu &operator=(const Emu &) = delete;
		Emu(Emu &&) = delete;
		Emu &operator=(Emu &&) = delete;

	private:
		void reset();
		void update();

		bool loadUromFile(const QString &path, CPU::UROM &urom);
		bool loadBiosFile(const QString &path, CPU::BIOS &bios);

		void ramSetPage(int page);

		void mousePressEvent(QMouseEvent *event) override;
		bool focusNextPrevChild(bool next) override;

		void keyPressEvent(QKeyEvent *event) override;

		Ui::Emu *ui; //!< Form for the emulator window

		bool loaded; //!< Status of loading all necessary files for emulation
		bool started; //!< Status of started the emulation process. It is "1" when the start button was clicked.
		bool running; //!< Status of running the emulation process. It is "1" when the emulation is active executing.

		CPU cpu; //!< CPU instance for emulating the processor
		IO io; //!< IO instance for emulating the motherboard

		int ramPage; //!< Number of the RAM page to show

	private slots:
		void updateSlot();

		void updateLEDRunSlot(bool enable);
		void updateLEDErrorSlot(bool enable);

		void updateRS232TxSlot(unsigned char c);
		void updateRS232RxSlot(unsigned char c);

		void updateRTCDateTimeSlot(const QDateTime &dateTime);

		void updateSpeakerStatusSlot(int bufferUsed);

		void on_fileUrom0OpenButton_clicked();
		void on_fileUrom1OpenButton_clicked();
		void on_fileBiosOpenButton_clicked();
		void on_fileFSDirOpenButton_clicked();

		void on_fileUrom0ReloadButton_clicked();
		void on_fileUrom1ReloadButton_clicked();
		void on_fileBiosReloadButton_clicked();

		void on_emuControlRunButton_clicked();
		void on_emuControlStepButton_clicked();
		void on_emuControlPauseButton_clicked();
		void on_emuControlStopButton_clicked();

		void on_speakerVolumeSlider_valueChanged(int value);

		void on_rtcSetCurrentButton_clicked();

		void on_ramGoToBIOSButton_clicked();
		void on_ramGoToOSButton_clicked();
		void on_ramGoToAppButton_clicked();
		void on_ramGoToSPButton_clicked();
		void on_ramMinusButton_clicked();
		void on_ramPlusButton_clicked();

		void on_rs232RxSendButton_clicked();
};

#endif
