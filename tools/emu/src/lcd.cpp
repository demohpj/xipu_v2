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

#include "lcd.h"

/**
 * Constructor for the LCD class
 *
 * @param parent Parent object
 */
LCD::LCD(QObject *parent) : QObject(parent)
{
	this->reset();
}

//! Reset the LCD buffers and clear the screen
void LCD::reset()
{
	this->clear();
	this->refresh();

	this->color = DEFAULT_COLOR;
	this->cursorPosX = 0;
	this->cursorPosY = 0;
}

//! Clear the canvas painting buffer
void LCD::clear()
{
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			this->buffer[0].charData[i][j] = 0;
			this->buffer[0].colorData[i][j] = 0;
		}
	}
}

//! Copy data from the canvas painting buffer to the ready to show buffer
void LCD::refresh()
{
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			this->buffer[1].charData[i][j] = this->buffer[0].charData[i][j];
			this->buffer[1].colorData[i][j] = this->buffer[0].colorData[i][j];
		}
	}
}

//! Scroll the canvas painting buffer one line up
void LCD::scrollDown()
{
	for(int y = 0; y < (HEIGHT - 1); y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			this->buffer[0].charData[y][x] = this->buffer[0].charData[y + 1][x];
			this->buffer[0].colorData[y][x] = this->buffer[0].colorData[y + 1][x];
		}
	}

	for(int x = 0; x < WIDTH; x++)
	{
		this->buffer[0].charData[HEIGHT - 1][x] = 0;
		this->buffer[0].colorData[HEIGHT - 1][x] = 0;
	}

	if(this->cursorPosY > 0)
	{
		this->cursorPosY--;
	}
}

//! Move the cursor to a new line
void LCD::newLine()
{
	this->cursorPosX = 0;

	if(this->cursorPosY >= HEIGHT)
	{
		this->scrollDown();
		this->cursorPosY = (HEIGHT - 1);
	}

	this->cursorPosY++;
}

/**
 * Set foreground color
 *
 * @param color Color
 */
void LCD::setColorForeground(unsigned char color)
{
	this->color = ((this->color & COLOR_MASK_BACKGROUND) | (color & COLOR_MASK_FOREGROUND));
}

/**
 * Set background color
 *
 * @param color Color
 */
void LCD::setColorBackground(unsigned char color)
{
	this->color = ((this->color & COLOR_MASK_FOREGROUND) | ((color << COLOR_SHIFT) & COLOR_MASK_BACKGROUND));
}

/**
 * Set horizontal cursor position
 *
 * @param x Horizontal position
 */
void LCD::setCursorPosX(unsigned char x)
{
	if(x < WIDTH)
	{
		this->cursorPosX = x;
	}
}

/**
 * Set vertical cursor position
 *
 * @param y Vertical position
 */
void LCD::setCursorPosY(unsigned char y)
{
	if(y < HEIGHT)
	{
		this->cursorPosY = y;
	}
}

/**
 * Get horizontal cursor position
 *
 * @return Horizontal position
 */
unsigned char LCD::getCursorPosX()
{
	return(this->cursorPosX);
}

/**
 * Get vertical cursor position
 *
 * @return Vertical position
 */
unsigned char LCD::getCursorPosY()
{
	return(this->cursorPosY);
}

/**
 * Print a character
 *
 * @param c Character to print
 */
void LCD::printChar(unsigned char c)
{
	if(this->cursorPosX >= WIDTH)
	{
		this->cursorPosX = 0;
		this->cursorPosY++;
	}

	if(this->cursorPosY >= HEIGHT)
	{
		this->scrollDown();
		this->cursorPosY = (HEIGHT - 1);
	}

	this->buffer[0].charData[this->cursorPosY][this->cursorPosX] = c;
	this->buffer[0].colorData[this->cursorPosY][this->cursorPosX] = this->color;

	this->cursorPosX++;
}

//! Emit the update signal with ready to show buffer data
void LCD::update()
{
	emit updateBufferSignal(this->buffer[1]);
}
