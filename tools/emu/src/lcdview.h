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

#ifndef LCDVIEW_H
#define LCDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QMouseEvent>
#include <QScopedPointer>

#include "lcd.h"

//! This class contains LCD view using to present LCD data on UI
class LCDView : public QGraphicsView
{
	Q_OBJECT

	public:
		static const int WIDTH = (LCD::CHAR_WIDTH * LCD::WIDTH); //!< Horizontal resolution
		static const int HEIGHT = (LCD::CHAR_HEIGHT * LCD::HEIGHT); //!< Vertical resolution

		LCDView(QWidget *parent = nullptr);

		void reset();

	private:
		QScopedPointer<QGraphicsScene> scene; //!< Scene used to rendering
		QGraphicsPixmapItem *pixmapItem; //!< Pixel buffer used as a canvas to paint the console

		void mousePressEvent(QMouseEvent *event) override;

	public slots:
		void drawSlot(LCD::Buffer buffer);
};

#endif
