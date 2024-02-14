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

#include <QApplication>
#include <QCommandLineParser>
#include <QFont>
#include <QFontDatabase>

#include "emu.h"

/**
 * Main entry function of the application
 *
 * @param argc Arguments count
 * @param argv Arguments list stored in an array
 */
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("emu");
	QApplication::setApplicationVersion("2.0.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("Emulator of the XiPC v2");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);

	QFontDatabase::addApplicationFont("fonts/DejaVuSansMono-Bold.ttf");
	QFont font = QApplication::font();

	font.setFamily("DejaVu Sans Mono");
	font.setPixelSize(11);
	font.setBold(true);

	QApplication::setFont(font, "QWidget");

	Emu emu;
	emu.show();

	return(QApplication::exec());
}
