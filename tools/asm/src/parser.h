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

#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QList>
#include <QRegularExpression>
#include <QFile>
#include <QFileInfo>
#include <QSharedPointer>

#include "codeitem.h"
#include "codesection.h"
#include "ramitem.h"
#include "flashitem.h"
#include "data.h"
#include "datalist.h"

//! This class parses all input source files for later compilation process
class Parser
{
	public:
		Parser(DataList &dataList);

		bool parseFile(const QString &path);

	private:
		DataList &dataList; //!< List of data for each source file

		bool parseCodeSectionEntry(int i, QString &label, QString &line, const QString &filePath, bool isExternal, bool isGlobal, QSharedPointer<Data> &data, QSharedPointer<CodeSection> &codeSection, bool &doNext);
		bool parseIncludeEntry(const QString &line, const QString &filePath, QList<QString> &fileList);
		bool parseMapEntry(int i, const QString &line, const QString &filePath, QList<QString> &mapList);
		bool parseRamEntry(int i, const QString &line, const QString &filePath, bool isExternal, bool isGlobal, QSharedPointer<Data> &data);
		bool parseFlashEntry(int i, const QString &line, const QString &filePath, bool isExternal, bool isGlobal, QSharedPointer<Data> &data);

		bool parseMap(int i, const QString &mapPath, const QStringList &lineItems);
		bool parseMapCodeSection(int i, const QString &mapPath, const QStringList &lineItems, QString &name, QString &tAddress);
		bool parseMapMemory(int i, const QString &mapPath, const QStringList &lineItems, QString &name, QString &tAddress);

		QSharedPointer<RamItem> parseRam(int i, const QString &line);
		QSharedPointer<FlashItem> parseFlash(int i, const QString &line);
		QSharedPointer<CodeItem> parseCode(int i, const QString &label, const QString &line);
};

#endif
