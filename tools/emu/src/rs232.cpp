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

#include "rs232.h"

/**
 * Constructor for the RS232 class
 *
 * @param parent Parent object
 */
RS232::RS232(QObject *parent) : QObject(parent)
{
	this->reset();
}

//! Reset the RS232 buffers
void RS232::reset()
{
	this->rxBuffer.clear();
}

/**
 * Receive function used to emulate input
 *
 * @param text Received text
 */
void RS232::rx(const QString &text)
{
	unsigned int charAdded = 0;

	for(const QChar &cFor : text)
	{
		unsigned char c = static_cast<unsigned char>(cFor.toLatin1());

		if((c >= MIN_CODE) && (c <= MAX_CODE))
		{
			this->rxBuffer.append(c);

			charAdded++;

			emit updateRxSignal(c);
		}
	}

	if(charAdded > 0)
	{
		this->rxBuffer.append(0x00);

		emit updateRxSignal(0x00);
	}

	if(this->rxBuffer.length() > 0)
	{
		emit dataReadySignal(true);
	}
}

/**
 * Send char
 *
 * @param c Char to send
 */
void RS232::send(unsigned char c)
{
	emit updateTxSignal(c & SEND_MASK);
}

/**
 * Read next a char from receive buffer
 *
 * @return Next char from the receive buffer
 */
unsigned char RS232::receive()
{
	if(this->rxBuffer.length() > 0)
	{
		return(this->rxBuffer.takeFirst());
	}

	emit dataReadySignal(false);

	return(0);
}

/**
 * Get free space size in the transmit buffer
 *
 * @return Free space size in the transmit buffer
 */
unsigned char RS232::getSendBufferFree()
{
	return(BUFFER_TX_SIZE);
}
