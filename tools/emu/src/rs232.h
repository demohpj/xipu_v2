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

#ifndef RS232_H
#define RS232_H

#include <QObject>
#include <QString>
#include <QList>

//! This class contains RS232 functions
class RS232 : public QObject
{
	Q_OBJECT

	public:
		static const int BUFFER_RX_SIZE = 64; //!< Receive buffer size
		static const int BUFFER_TX_SIZE = 64; //!< Transmit buffer size

		static const int SEND_MASK = 0x7f; //!< Mask to make a basic ASCII char set

		static const int MAX_CODE = 0x7f; //!< Maximum allowed ASCII char
		static const int MIN_CODE = 0x20; //!< Minimum allowed ASCII char

		RS232(QObject *parent = nullptr);

		void reset();

		void rx(const QString &text);

		void send(unsigned char c);
		unsigned char receive();

		unsigned char getSendBufferFree();

	private:
		QList<unsigned char> rxBuffer; //!< Receive buffer

	signals:
		void updateTxSignal(unsigned char c);
		void updateRxSignal(unsigned char c);

		void dataReadySignal(bool ready);
};

#endif
