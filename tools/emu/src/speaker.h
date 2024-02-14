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

#ifndef SPEAKER_H
#define SPEAKER_H

#include <QObject>
#include <QList>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QByteArray>
#include <QScopedPointer>

//! This class contains speaker functions
class Speaker : public QObject
{
	Q_OBJECT

	public:
		static const int SAMPLE_RATE = 44100; //!< Sampple rate in Hz

		static const int BUFFER_SIZE = 32; //!< Size of the buffer

		static const int NOTE_QUANTITY = 89; //!< Note quantity available to play

		static const int TIME_MIN = 1; //!< Minimum time to play for a note in ms multiplied by TIME_MUL
		static const int TIME_MAX = 250; //!< Maximum time to play for a note in ms multiplied by TIME_MUL
		static constexpr double TIME_MUL = 10.0; //!< Time multiplicator for a note

		static const int FILL_MIN = 1; //!< Minimum fill of the square wave in percents divided by FILL_DIV
		static const int FILL_MAX = 15; //!< Maximum fill of the square wave in percents divided by FILL_DIV
		static const int FILL_MASK = 0x0f; //!< Mask for the fill value
		static constexpr double FILL_DIV = 16.0; //!< Square wave fill divider

		static const int VOLUME_MASK = 0x0f; //!< Mask for the volume value
		static constexpr double VOLUME_MAX = 15.0; //!< Maximum volume level

		static constexpr double VALUE_MUL = 127.0; //!< Output level multiplicator used to conversion
		static constexpr double VALUE_VOLUME = 0.3; //!< Output volume level multiplicator

		//! Fields used by speaker to describe note
		struct Note
		{
			unsigned char note; //!< Number of the note
			unsigned char time; //!< Time to play
			unsigned char fill; //!< Square wave fill
			unsigned char volume; //!< Volume level
		};

		Speaker(QObject *parent = nullptr);
		~Speaker() override;

		Speaker(const Speaker &) = delete;
		Speaker &operator=(const Speaker &) = delete;
		Speaker(Speaker &&) = delete;
		Speaker &operator=(Speaker &&) = delete;

		void reset();

		void play();
		void pause();
		void clear();
		bool isPlaying();

		void setVolume(unsigned int volume);

		void addNote(const Speaker::Note &note);
		unsigned char getBufferFree();

	private:
		//! List of possible notes to play
		enum NoteCode
		{
			NOTE_SILENT,
			NOTE_A0, NOTE_AS0, NOTE_B0,
			NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1,
			NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2,
			NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
			NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
			NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
			NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
			NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
			NOTE_C8
		};

		//! List of frequencies for notes
		QVector<int> noteFrequency = QVector<int>({
			20, // Silent
			27, 29, 31, //  A0 - B0
			33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, // C1 - B1
			65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123, // C2 - B2
			131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, // C3 - B3
			262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, // C4 - B4
			523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, // C5 - B5
			1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, // C6 - B6
			2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, // C7 - B7
			4186 // C8
		});

		bool playing; //!< Playing status
		QList<struct Note> buffer; //!< Buffer of notes

		QAudioFormat audioFormat; //!< Audio stream parameter information
		QScopedPointer<QAudioOutput> audioOutput; //!< Interface to an audio output device
		QIODevice *ioDevice; //!< IO device retuned by QAudioOutput

		QByteArray bufferOut; //!< Raw buffer with prepared data ready to send to IO device

		void playNextNote();

	signals:
		void updateStatusSignal(int bufferUsed);

	private slots:
		void audioNotifySlot();
};

#endif
