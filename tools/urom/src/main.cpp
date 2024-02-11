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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>

#include "urom.h"

static const QString UROM0_PATH = "urom0.bin"; //!< Path to the first microcode output file
static const QString UROM1_PATH = "urom1.bin"; //!< Path to the second microcode output file

//! Main entry function of the application
int main(int argc, char *argv[])
{
	QTextStream out(stdout);

	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("urom");
	QCoreApplication::setApplicationVersion("2.0.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("uROM generator for the XiPU v2");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);

	// Create and generate the microcode
	URom uRom = URom();

	// Try to generate the data
	if(!uRom.generateData())
	{
		out << "ERROR: Failed to generate uROM data\n";

		// Return the error code
		return(-1);
	}

	// Try to save the data to the output files
	if(!uRom.saveFiles(UROM0_PATH, UROM1_PATH))
	{
		// If failed show the error message
		out << "ERROR: Failed to save uROM data\n";

		// Return the error code
		return(-2);
	}

	// The data files were saved. Show the success message
	out << "OK: uROM data has been saved successfully\n";

	// Return the success code
	return(0);
}
