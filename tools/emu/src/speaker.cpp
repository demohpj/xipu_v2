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

#include "speaker.h"

/**
 * Constructor for the speaker class
 *
 * @param parent Parent object
 */
Speaker::Speaker(QObject *parent) : QObject(parent)
{
	this->audioFormat.setSampleRate(SAMPLE_RATE);
	this->audioFormat.setSampleSize(16);
	this->audioFormat.setChannelCount(1);
	this->audioFormat.setCodec("audio/pcm");
	this->audioFormat.setByteOrder(QAudioFormat::LittleEndian);
	this->audioFormat.setSampleType(QAudioFormat::SignedInt);

	this->audioOutput.reset(new QAudioOutput(this->audioFormat, this));
	this->audioOutput->setNotifyInterval(50);

	QObject::connect(this->audioOutput.data(), SIGNAL(notify()), this, SLOT(audioNotifySlot()));

	this->reset();
}

//! Destructor for the speaker class
Speaker::~Speaker()
{
	this->reset();

	QObject::disconnect(this);
}

//! Reset the speaker buffers and stop playing sound
void Speaker::reset()
{
	this->clear();
}

//! Start playing sound
void Speaker::play()
{
	if((!this->playing) && (!this->buffer.empty()))
	{
		this->playNextNote();
	}
}

//! Pause playing sound
void Speaker::pause()
{
	if(!this->playing)
	{
	   this->playing = false;
	   this->audioOutput->stop();
	}
}

//! Clear the speaker buffers and stop playing sound
void Speaker::clear()
{
	this->playing = false;

	this->audioOutput->reset();
	this->buffer.clear();

	this->bufferOut.clear();
	this->audioOutput->stop();

	this->ioDevice = this->audioOutput->start();
}

/**
 * Get status of playing sound
 *
 * @return Status of playing sound
 */
bool Speaker::isPlaying()
{
	return(this->playing);
}

/**
 * Set volume
 *
 * @param volume Volume level in range 0-100
 */
void Speaker::setVolume(unsigned int volume)
{
	if(volume <= 100)
	{
		this->audioOutput->setVolume(static_cast<qreal>(volume) / 100.0);
	}
}

/**
 * Add note to the buffer
 *
 * @param note Note to add to the buffer
 */
void Speaker::addNote(const Speaker::Note &note)
{
	if(note.note >= NOTE_QUANTITY)
	{
		return;
	}

	if((note.time < TIME_MIN) || (note.time > TIME_MAX))
	{
		return;
	}

	if(note.note != NOTE_SILENT)
	{
		if((note.fill < FILL_MIN) || (note.fill > FILL_MAX))
		{
			return;
		}

		if(note.volume > VOLUME_MAX)
		{
			return;
		}
	}

	Note noteItem = {};

	noteItem.note = note.note;
	noteItem.time = note.time;

	if(note.note == NOTE_SILENT)
	{
		noteItem.fill = FILL_MIN;
		noteItem.volume = 0;
	}
	else
	{
		noteItem.fill = (note.fill & FILL_MASK);
		noteItem.volume = (note.volume & VOLUME_MASK);
	}

	this->buffer.append(noteItem);

	if(this->playing)
	{
		emit updateStatusSignal(this->buffer.length());
	}
}

/**
 * Get free space size in the buffer
 *
 * @return Free space size in the buffer
 */
unsigned char Speaker::getBufferFree()
{
	return(static_cast<unsigned char>(BUFFER_SIZE - this->buffer.length()));
}

//! Load next note to play from the buffer
void Speaker::playNextNote()
{
	if(!this->buffer.empty())
	{
		struct Note noteItem = this->buffer.takeFirst();

		char signalLevel = static_cast<char>(VALUE_MUL * VALUE_VOLUME * (static_cast<double>(noteItem.volume) / VOLUME_MAX));

		char high = ((noteItem.note == NoteCode::NOTE_SILENT) ? 0 : signalLevel);
		char low = 0;

		double duration = (static_cast<double>(noteItem.time) * TIME_MUL);
		double fill = (static_cast<double>(noteItem.fill) / FILL_DIV);

		double atomicTime = (1000.0 / SAMPLE_RATE);
		double sqareTime = (1000.0 / static_cast<double>(noteFrequency[static_cast<int>(noteItem.note)]));
		double time = 0.0;

		QByteArray data;

		for(double i = 0.0; i < (duration - 0.01); i += atomicTime)
		{
			data.append(static_cast<char>(0));
			data.append((time < (sqareTime * fill)) ? high : low);

			if(time >= sqareTime)
			{
				time = 0.0;
			}
			else
			{
				time += atomicTime;
			}
		}

		this->bufferOut.append(data);

		emit updateStatusSignal(this->buffer.length());

		if(!this->playing)
		{
			this->playing = true;

			this->audioNotifySlot();
		}
	}
	else
	{
		this->playing = false;
	}
}

//! Load next raw data to play to the audio output
void Speaker::audioNotifySlot()
{
	if(this->playing)
	{
		int freeBytes = this->audioOutput->bytesFree();

		if(freeBytes > 0)
		{
			int bufferBytes = this->bufferOut.length();
			int bytesToWrite = qMin(bufferBytes, freeBytes);

			if(bufferBytes > 0)
			{
				this->ioDevice->write(this->bufferOut.left(bytesToWrite));
				this->bufferOut.remove(0, bytesToWrite);

				if(freeBytes > bytesToWrite)
				{
					this->playNextNote();
					this->audioNotifySlot();
				}
			}
			else
			{
				this->playNextNote();
				this->audioNotifySlot();
			}
		}
	}
}
