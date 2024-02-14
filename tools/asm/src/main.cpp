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
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "asm.h"

/**
 * Main entry function of the application
 *
 * @param argc Arguments count
 * @param argv Arguments list stored in an array
 */
int main(int argc, char *argv[])
{
	QTextStream out(stdout);

	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("asm");
	QCoreApplication::setApplicationVersion("2.0.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("Assembler compilator for the XiPU v2");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("mode", R"(Mode selection of compilation: "bios", "os", "app")");
	parser.addPositionalArgument("input_file", "Assembler source input file");
	parser.addPositionalArgument("output_bin", "Binary output file");
	parser.addPositionalArgument("output_map", R"(External functions map output file - only for: "bios" and "os")");
	parser.process(app);

	const QStringList args = parser.positionalArguments();

	// Prepare the assembler
	Asm assembler = Asm();
	bool status = false;

	if((args.size() == 4) && (args.at(0) == "bios"))
	{
		status = assembler.compileBios(args.at(1), args.at(2), args.at(3));
	}
	else if((args.size() == 4) && (args.at(0) == "os"))
	{
		status = assembler.compileOs(args.at(1), args.at(2), args.at(3));
	}
	else if((args.size() == 3) && (args.at(0) == "app"))
	{
		status = assembler.compileApp(args.at(1), args.at(2));
	}
	else
	{
		out << "ERROR: Unknow compilation mode or bad number of arguments" << "\n\n";
		out.flush();

		parser.showHelp(-2);
	}

	return(status ? 0 : -1);
}
