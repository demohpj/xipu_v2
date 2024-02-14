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

#ifndef LCD_H
#define LCD_H

#include <QObject>
#include <QVector>

#ifdef CHAR_WIDTH
#undef CHAR_WIDTH
#endif

//! This class contains LCD functions and buffer
class LCD : public QObject
{
	Q_OBJECT

	public:
		static const int CHAR_WIDTH = 8; //!< Width of the char in pixels
		static const int CHAR_HEIGHT = 8; //!< Height of the char in pixels

		static const int WIDTH = 40; //!< Chars per row
		static const int HEIGHT = 30; //!< Chars per column

		static const int COLOR_MASK_FOREGROUND = 0x0f; //!< Foreground color mask
		static const int COLOR_MASK_BACKGROUND = 0xf0; //!< Background color mask
		static const int COLOR_SHIFT = 4; //!< Char color shift, background color is high 4 bits, foreground color is low 4 bits

		static const unsigned char DEFAULT_COLOR = 0x0f; //!< Default foreground and background colors

		//! LCD buffer
		struct Buffer
		{
			QVector<QVector<unsigned char>> charData = QVector<QVector<unsigned char>>(HEIGHT, QVector<unsigned char>(WIDTH)); //!< Character buffer
			QVector<QVector<unsigned char>> colorData = QVector<QVector<unsigned char>>(HEIGHT, QVector<unsigned char>(WIDTH)); //!< Background and foreground color buffer
		};

		LCD(QObject *parent = nullptr);

		void reset();

		void clear();
		void refresh();
		void scrollDown();
		void newLine();

		void setColorForeground(unsigned char color);
		void setColorBackground(unsigned char color);
		void setCursorPosX(unsigned char x);
		void setCursorPosY(unsigned char y);

		unsigned char getCursorPosX();
		unsigned char getCursorPosY();

		void printChar(unsigned char c);

		void update();

	private:
		QVector<Buffer> buffer = { Buffer(), Buffer() }; //!< First buffer is using as canvas for painting, the second one is a copy ready to show

		unsigned char color; //!< Current set foreground and background colors
		unsigned char cursorPosX; //!< Current horizontal cursor position
		unsigned char cursorPosY; //!< Current vertical cursor position

	signals:
		void updateBufferSignal(LCD::Buffer buffer);
};

#endif
