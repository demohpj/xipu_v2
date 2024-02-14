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

#include "keyboard.h"

/**
 * Constructor for the keyboard class
 *
 * @param parent Parent object
 */
Keyboard::Keyboard(QObject *parent) : QObject(parent)
{
	this->reset();
}

//! Reset the keyboard buffer
void Keyboard::reset()
{
	this->buffer.clear();
}

/**
 * Process a key press event
 *
 * @param key Pressed key code
 * @param modifiers Optional modifiers to pressed key
 */
void Keyboard::keyPress(int key, Qt::KeyboardModifiers modifiers)
{
	if((key <= MAX_CODE) && (key >= MIN_CODE))
	{
		unsigned char keyCode = static_cast<unsigned char>(key);

		if((key >= 'A') && (key <= 'Z'))
		{
			if(!(modifiers & Qt::ShiftModifier))
			{
				keyCode += ('a' - 'A');
			}
		}

		if(modifiers & Qt::ControlModifier)
		{
			keyCode |= CTRL_MASK;
		}

		this->buffer.append(keyCode);
	}
	else
	{
		switch(key)
		{
			case Qt::Key_Escape :
				this->buffer.append(KEY_ESC);
				break;

			case Qt::Key_Backspace :
				this->buffer.append(KEY_BACKSPACE);
				break;

			case Qt::Key_Tab :
				this->buffer.append(KEY_TAB);
				break;

			case Qt::Key_Return :
				this->buffer.append(KEY_ENTER);
				break;

			case Qt::Key_Delete :
				this->buffer.append(KEY_DELETE);
				break;

			case Qt::Key_Up :
				this->buffer.append(KEY_UP);
				break;

			case Qt::Key_Down :
				this->buffer.append(KEY_DOWN);
				break;

			case Qt::Key_Left :
				this->buffer.append(KEY_LEFT);
				break;

			case Qt::Key_Right :
				this->buffer.append(KEY_RIGHT);
				break;
		}
	}

	if(this->buffer.length() > 0)
	{
		emit dataReadySignal(true);
	}
}

/**
 * Read next a char from input buffer
 *
 * @return Next char from the input buffer
 */
unsigned char Keyboard::getKey()
{
	if(this->buffer.length() > 0)
	{
		unsigned char key = this->buffer.takeFirst();

		if(this->buffer.length() == 0)
		{
			emit dataReadySignal(false);
		}

		return(key);
	}

	emit dataReadySignal(false);

	return(0);
}
