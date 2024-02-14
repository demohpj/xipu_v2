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

#include "lcdview.h"
#include "font.h"

/**
 * Constructor for the LCD view class
 *
 * @param parent Parent object
 */
LCDView::LCDView(QWidget *parent) : QGraphicsView(parent)
{
	this->scene.reset(new QGraphicsScene(QRect(0, 0, WIDTH, HEIGHT), this));

	QPixmap pixmap = QPixmap(WIDTH, HEIGHT);
	pixmap.fill(QColor(0, 0, 0));

	this->pixmapItem = this->scene->addPixmap(pixmap);

	this->scale(2, 2);
	this->setScene(this->scene.data());
	this->setBackgroundBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));
	this->show();
}

//! Reset the LCD view pixmap canvas
void LCDView::reset()
{
	QImage image = QImage(WIDTH, HEIGHT, QImage::Format_RGB32);
	image.fill(QColor(0, 0, 0));

	this->pixmapItem->setPixmap(QPixmap::fromImage(image));
}

/**
 * Process a mouse press event
 *
 * @param event Mouse press event
 */
void LCDView::mousePressEvent(QMouseEvent *)
{
	if(this->parentWidget())
	{
		this->parentWidget()->setFocus();
	}
}

/**
 * Draw a LCD buffer on the LCD view canvas
 *
 * @param buffer LCD buffer
 */
void LCDView::drawSlot(LCD::Buffer buffer)
{
	QImage image = QImage(WIDTH, HEIGHT, QImage::Format_RGB32);
	image.fill(QColor(0, 0, 0));

	for(int x = 0; x < LCD::WIDTH; x++)
	{
		for(int y = 0; y < LCD::HEIGHT; y++)
		{
			unsigned char c = buffer.charData[y][x];
			unsigned char colorChar = (buffer.colorData[y][x] & LCD::COLOR_MASK_FOREGROUND);
			unsigned char colorBackground = ((buffer.colorData[y][x] & LCD::COLOR_MASK_BACKGROUND) >> LCD::COLOR_SHIFT);

			unsigned char colorCharBlue = ((colorChar & (1 << 0)) ? 170 : 0) + ((colorChar & (1 << 3)) ? 85 : 0);
			unsigned char colorCharGreen = ((colorChar & (1 << 1)) ? 180 : 0) + ((colorChar & (1 << 3)) ? 75 : 0);
			unsigned char colorCharRed = ((colorChar & (1 << 2)) ? 180 : 0) + ((colorChar & (1 << 3)) ? 75 : 0);

			unsigned char colorBackgroundBlue = ((colorBackground & (1 << 0)) ? 170 : 0) + ((colorBackground & (1 << 3)) ? 85 : 0);
			unsigned char colorBackgroundGreen = ((colorBackground & (1 << 1)) ? 180 : 0) + ((colorBackground & (1 << 3)) ? 75 : 0);
			unsigned char colorBackgroundRed = ((colorBackground & (1 << 2)) ? 180 : 0) + ((colorBackground & (1 << 3)) ? 75 : 0);

			for(int j = 0; j < LCD::CHAR_HEIGHT; j++)
			{
				unsigned char b = fontTable[c][j];

				for(int i = 0; i < LCD::CHAR_WIDTH; i++)
				{
					if(b & 0x80)
					{
						image.setPixelColor(((x * LCD::CHAR_WIDTH) + i), ((y * LCD::CHAR_HEIGHT) + j), QColor(colorCharRed, colorCharGreen, colorCharBlue));
					}
					else
					{
						image.setPixelColor(((x * LCD::CHAR_WIDTH) + i), ((y * LCD::CHAR_HEIGHT) + j), QColor(colorBackgroundRed, colorBackgroundGreen, colorBackgroundBlue));
					}

					b <<= 1;
				}
			}
		}
	}

	this->pixmapItem->setPixmap(QPixmap::fromImage(image));
}
