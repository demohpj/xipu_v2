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

#ifndef LINKER_H
#define LINKER_H

#include <QString>
#include <QTextStream>
#include <QVector>
#include <QHash>
#include <QRegularExpression>
#include <QFile>
#include <QIODevice>

#include "codeitem.h"
#include "codesection.h"
#include "ramitem.h"
#include "flashitem.h"
#include "data.h"
#include "datalist.h"

//! This class glues all binary parts from the compilation part and set correct memory addresses
class Linker
{
	public:
		static const int BIOS_RAM_SIZE = 0x0800; //!< Size of RAM reserved for the BIOS
		static const int BIOS_RAM_OFFSET = 0x0000; //!< Start of RAM reserved for the BIOS

		static const int OS_RAM_SIZE = 0x1800; //!< Size of RAM reserved for the OS
		static const int OS_RAM_OFFSET = 0x0800; //!< Start of RAM reserved for the OS

		static const int APP_RAM_SIZE = 0xD000; //!<  Size of RAM reserved for an user application
		static const int APP_RAM_OFFSET = 0x2000; //!< Start of RAM reserved for an user application

		Linker(DataList &dataList);

		bool link();
		bool saveFile(const QString &path, const QString &pathMap);

	private:
		DataList &dataList; //!< List of data for each source file
		QVector<unsigned char> flashVector; //!< Buffer with a compiled, parsed and linked binary code

		bool findAddress(Data &data, const QString &label, unsigned int &address);
		bool findCodeAddress(Data &data, const QString &name, unsigned int &address);
		bool findMemoryAddress(Data &data, const QString &name, unsigned int &address);
};

#endif
