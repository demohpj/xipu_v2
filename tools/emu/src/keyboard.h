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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QList>

//! This class contains keyboard functions
class Keyboard : public QObject
{
	Q_OBJECT

	public:
		static const int KEY_ESC = 0x1b; //!< Escape key code
		static const int KEY_BACKSPACE = 0x08; //!< Backspace key code
		static const int KEY_TAB = 0x09; //!< Tab key code
		static const int KEY_ENTER = 0x0d; //!< Enter key code
		static const int KEY_DELETE = 0x7f; //!< Delete key code
		static const int KEY_UP = 0x11; //!< Up arrow key code
		static const int KEY_DOWN = 0x12; //!< Down arrow key code
		static const int KEY_LEFT = 0x13; //!< Left arrow key code
		static const int KEY_RIGHT = 0x14; //!< Right arrow key code

		static const int CTRL_MASK = 0x80; //!< Mask used to check if the control key is pressed too

		static const int MIN_CODE = 0x20; //!< Minimum correctly key code
		static const int MAX_CODE = 0x7e; //!< Maximum correctly key code

		Keyboard(QObject *parent = nullptr);

		void reset();

		void keyPress(int key, Qt::KeyboardModifiers modifiers);

		unsigned char getKey();

	private:
		QList<unsigned char> buffer; //!< Input key buffer

	signals:
		void dataReadySignal(bool ready);
};

#endif
