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

#include "parser.h"

/**
 * Constructor of the parser
 *
 * @param dataList Data list
 */
Parser::Parser(DataList &dataList) : dataList(dataList)
{
}

/**
 * Parse a file
 *
 * @param path Path to the file
 *
 * @return Status of parsing the file
 */
bool Parser::parseFile(const QString &path)
{
	QTextStream out(stdout);
	QList<QString> fileList;
	QList<QString> mapList;

	fileList.append(QFileInfo(path).absoluteFilePath());

	for(int i = 0; i < fileList.size(); i++)
	{
		const QString &filePath = fileList.at(i);
		QFile file(filePath);
		QStringList lineList;

		if(!file.open(QIODevice::ReadOnly))
		{
			out << QString("ERROR: Cannot open file: \"%1\"\n").arg(filePath);
			return(false);
		}

		while(!file.atEnd())
		{
			lineList.append(file.readLine().trimmed());
		}

		file.close();

		QSharedPointer<Data> data(new Data(filePath));
		QSharedPointer<CodeSection> codeSection(new CodeSection(""));

		for(int j = 0; j < lineList.size(); j++)
		{
			// How many additional lines are glued in multiline
			int g = 0;
			QString line = "";
			int offset;

			// Glue lines if needed
			do
			{
				// Trimm line
				QString lineTemp = lineList.at(j + g);
				offset = lineTemp.indexOf(QRegularExpression("(?<!\\\\);|^;"));

				if(offset >= 0)
				{
					lineTemp = lineTemp.left(offset).trimmed();
				}

				// Clear line in list if something was glued previous
				if(g > 0)
				{
					lineList.replace((j + g), "");
				}

				// Glue lines if multiline style was used
				if(lineTemp.right(1) == '\\')
				{
					line.append(lineTemp.leftRef(lineTemp.length() - 1));
					g++;
				}
				else
				{
					line.append(lineTemp);
					break;
				}
			} while((j + g) < lineList.size());

			// Ignore empty lines
			if(line.length() <= 0)
			{
				continue;
			}

			// Find label
			offset = line.indexOf(QRegularExpression("\\s+"));
			QString label;

			if(offset > 0)
			{
				label = line.left(offset).trimmed();
			}
			else
			{
				label = line;
			}

			line = line.mid(label.length()).trimmed();

			// Find external or global
			bool isExternal = false;
			bool isGlobal = false;

			if(!label.compare("external", Qt::CaseInsensitive))
			{
				if((this->dataList.getMode() == DataList::Mode::Bios) || (this->dataList.getMode() == DataList::Mode::Os))
				{
					isExternal = true;
				}
				else
				{
					out << QString("ERROR: Special keyword \"external\" is not allowed in APP mode, found at line %1 in file: \"%2\"\n").arg(j + 1).arg(filePath);
					return(false);
				}
			}

			if(!label.compare("global", Qt::CaseInsensitive))
			{
				isGlobal = true;
			}

			if(isExternal || isGlobal)
			{
				offset = line.indexOf(QRegularExpression("\\s+"));

				if(offset > 0)
				{
					label = line.left(offset).trimmed();
				}
				else
				{
					label = line;
				}

				line = line.mid(label.length()).trimmed();
			}

			// Parse code section
			if((label.at(0) == ".") && (label.right(1) == ":"))
			{
				bool doNext = false;

				if(!(this->parseCodeSectionEntry(j, label, line, filePath, isExternal, isGlobal, data, codeSection, doNext)))
				{
					return(false);
				}

				if(doNext)
				{
					continue;
				}
			}

			// Parse include
			if(!label.compare("include", Qt::CaseInsensitive))
			{
				if(!(this->parseIncludeEntry(line, filePath, fileList)))
				{
					return(false);
				}

				continue;
			}

			// Parse map
			if(!label.compare("map", Qt::CaseInsensitive))
			{
				if(!(this->parseMapEntry(j, line, filePath, mapList)))
				{
					return(false);
				}

				continue;
			}

			// Parse ram
			if(!label.compare("var", Qt::CaseInsensitive))
			{
				if(!(this->parseRamEntry(j, line, filePath, isExternal, isGlobal, data)))
				{
					return(false);
				}

				continue;
			}

			// Parse flash
			if(!label.compare("const", Qt::CaseInsensitive))
			{
				if(!(this->parseFlashEntry(j, line, filePath, isExternal, isGlobal, data)))
				{
					return(false);
				}

				continue;
			}

			// Parse code
			QSharedPointer<CodeItem> codeItem = this->parseCode(j, label, line);

			if(!codeItem.isNull())
			{
				codeSection->addCodeItem(codeItem);
			}
			else
			{
				out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(j + 1).arg(filePath);
				return(false);
			}
		}

		if(!data->addCodeSection(codeSection))
		{
			out << QString("ERROR: Reuse of section name: \"%1\", found in file: \"%2\"\n").arg(codeSection->getName(), filePath);
			return(false);
		}

		this->dataList.append(data);
	}

	return(true);
}

/**
 * Parse a code section entry
 *
 * @param i Line number
 * @param label Label name
 * @param line Line of code
 * @param filePath Path to the parsed file
 * @param isExternal Switch to set the label to be visible external
 * @param isGlobal Switch to set the label to be visible global
 * @param data Data container where all parsed items are going in
 * @param codeSection Current selected code section data container
 * @param doNext Do next parse of the line if any data left
 *
 * @return Status of parsing the entry
 */
bool Parser::parseCodeSectionEntry(int i, QString &label, QString &line, const QString &filePath, bool isExternal, bool isGlobal, QSharedPointer<Data> &data, QSharedPointer<CodeSection> &codeSection, bool &doNext)
{
	QTextStream out(stdout);
	int offset;

	if(label.mid(1, (label.length() - 2)).contains(QRegularExpression("^([0-9a-zA-Z_-]+)$")))
	{
		if(!data->addCodeSection(codeSection))
		{
			out << QString("ERROR: Reuse of section name: \"%1\", found at line %2 in file: \"%3\"\n").arg(codeSection->getName()).arg(i + 1).arg(filePath);
			return(false);
		}

		codeSection = QSharedPointer<CodeSection>(new CodeSection(label.left(label.length() - 1)));

		if(isExternal)
		{
			if(!this->dataList.addExternalCodeSection(codeSection))
			{
				out << QString("ERROR: Reuse of external section name: \"%1\", found at line %2 in file: \"%3\"\n").arg(codeSection->getName()).arg(i + 1).arg(filePath);
				return(false);
			}
		}
		else if(isGlobal)
		{
			if(!this->dataList.addGlobalCodeSection(codeSection))
			{
				out << QString("ERROR: Reuse of global section name: \"%1\", found at line %2 in file: \"%3\"\n").arg(codeSection->getName()).arg(i + 1).arg(filePath);
				return(false);
			}
		}

		if(line.length() == 0)
		{
			doNext = true;
			return(true);
		}

		offset = line.indexOf(QRegularExpression("\\s+"));

		if(offset > 0)
		{
			label = line.left(offset).trimmed();
		}
		else
		{
			label = line;
		}

		line = line.mid(label.length()).trimmed();
	}
	else
	{
		out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(i + 1).arg(filePath);
		return(false);
	}

	return(true);
}

/**
 * Parse an include entry
 *
 * @param line Line of code
 * @param filePath Path to the parsed file
 * @param fileList List of all included files
 *
 * @return Status of parsing the entry
 */
bool Parser::parseIncludeEntry(const QString &line, const QString &filePath, QList<QString> &fileList)
{
	QString tPath;

	if(QFileInfo(line).isAbsolute())
	{
		tPath = line;
	}
	else
	{
		tPath = QFileInfo(QFileInfo(filePath).absolutePath().append("/").append(line)).absoluteFilePath();
	}

	bool exists = false;

	for(int j = 0; j < this->dataList.size(); j++)
	{
		if(this->dataList.at(j)->getFileName() == tPath)
		{
			exists = true;
			break;
		}
	}

	if(!exists)
	{
		for(const QString &fileFor : fileList)
		{
			if(fileFor == tPath)
			{
				exists = true;
				break;
			}
		}
	}

	if(!exists)
	{
		fileList.append(tPath);
	}

	return(true);
}

/**
 * Parse a map entry
 *
 * @param i Line number
 * @param line Line of code
 * @param filePath Path to the parsed file
 * @param mapList List of all map files
 *
 * @return Status of parsing the entry
 */
bool Parser::parseMapEntry(int i, const QString &line, const QString &filePath, QList<QString> &mapList)
{
	QTextStream out(stdout);

	if(this->dataList.getMode() == DataList::Mode::Bios)
	{
		out << QString("ERROR: Special keyword \"map\" is not allowed in BIOS mode, found at line %1 in file: \"%2\"\n").arg(i + 1).arg(filePath);
		return(false);
	}

	QString tPath;

	if(QFileInfo(line).isAbsolute())
	{
		tPath = line;
	}
	else
	{
		tPath = QFileInfo(QFileInfo(filePath).absolutePath().append("/").append(line)).absoluteFilePath();
	}

	bool exists = false;

	for(int k = 0; k < this->dataList.size(); k++)
	{
		if(this->dataList.at(k)->getFileName() == tPath)
		{
			exists = true;
			break;
		}
	}

	if(!exists)
	{
		for(const QString &mapFor : mapList)
		{
			if(mapFor == tPath)
			{
				exists = true;
				break;
			}
		}
	}

	if(!exists)
	{
		mapList.append(tPath);
	}
	else
	{
		return(true);
	}

	QFile file(tPath);
	QStringList lineList;

	if(!file.open(QIODevice::ReadOnly))
	{
		out << QString("ERROR: Cannot open file: \"%1\"\n").arg(tPath);
		return(false);
	}

	while(!file.atEnd())
	{
		lineList.append(file.readLine().trimmed());
	}

	file.close();

	for(int j = 0; j < lineList.size(); j++)
	{
		// Trimm line
		QString line = lineList.at(j);
		int offset = line.indexOf(QRegularExpression("(?<!\\\\);|^;"));

		if(offset >= 0)
		{
			line = line.left(offset).trimmed();
		}

		if(line.length() <= 0)
		{
			return(true);
		}

		QStringList lineItems = line.split(QRegularExpression("\\s+"));

		if(!this->parseMap(j, tPath, lineItems))
		{
			return(false);
		}
	}

	return(true);
}

/**
 * Parse a ram entry
 *
 * @param i Line number
 * @param line Line of code
 * @param filePath Path to the parsed file
 * @param isExternal Switch to set the label to be visible external
 * @param isGlobal Switch to set the label to be visible global
 * @param data Data container where all parsed items are going in
 *
 * @return Status of parsing the entry
 */
bool Parser::parseRamEntry(int i, const QString &line, const QString &filePath, bool isExternal, bool isGlobal, QSharedPointer<Data> &data)
{
	QTextStream out(stdout);

	if(this->dataList.getMode() == DataList::Mode::Bios)
	{
		out << QString("ERROR: Special keyword \"var\" is not allowed in BIOS mode, found at line %1 in file: \"%2\"\n").arg(i + 1).arg(filePath);
		return(false);
	}

	QSharedPointer<RamItem> ramItem = this->parseRam(i, line);

	if(!ramItem.isNull())
	{
		if(!data->addRamItem(ramItem))
		{
			out << QString("ERROR: Reuse of name: \"%1\", found at line %2 in file: \"%3\"\n").arg(ramItem->getName()).arg(i + 1).arg(filePath);
			return(false);
		}

		if(isExternal)
		{
			if(!this->dataList.addExternalRamItem(ramItem))
			{
				out << QString("ERROR: Reuse of external name: \"%1\", found at line %2 in file: \"%3\"\n").arg(ramItem->getName()).arg(i + 1).arg(filePath);
				return(false);
			}
		}
		else if(isGlobal)
		{
			if(!this->dataList.addGlobalRamItem(ramItem))
			{
				out << QString("ERROR: Reuse of global name: \"%1\", found at line %2 in file: \"%3\"\n").arg(ramItem->getName()).arg(i + 1).arg(filePath);
				return(false);
			}
		}
	}
	else
	{
		out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(i + 1).arg(filePath);
		return(false);
	}

	return(true);
}

/**
 * Parse a flash entry
 *
 * @param i Line number
 * @param line Line of code
 * @param filePath Path to the parsed file
 * @param isExternal Switch to set the label to be visible external
 * @param isGlobal Switch to set the label to be visible global
 * @param data Data container where all parsed items are going in
 *
 * @return Status of parsing the entry
 */
bool Parser::parseFlashEntry(int i, const QString &line, const QString &filePath, bool isExternal, bool isGlobal, QSharedPointer<Data> &data)
{
	QTextStream out(stdout);
	QSharedPointer<FlashItem> flashItem = this->parseFlash(i, line);

	if(!flashItem.isNull())
	{
		if(!data->addFlashItem(flashItem))
		{
			out << QString("ERROR: Reuse of name: \"%1\", found at line %2 in file: \"%3\"\n").arg(flashItem->getName()).arg(i + 1).arg(filePath);
			return(false);
		}

		if(isExternal)
		{
			if(!this->dataList.addExternalFlashItem(flashItem))
			{
				out << QString("ERROR: Reuse of external name: \"%1\", found at line %2 in file: \"%3\"\n").arg(flashItem->getName()).arg(i + 1).arg(filePath);
				return(false);
			}
		}
		else if(isGlobal)
		{
			if(!this->dataList.addGlobalFlashItem(flashItem))
			{
				out << QString("ERROR: Reuse of global name: \"%1\", found at line %2 in file: \"%3\"\n").arg(flashItem->getName()).arg(i + 1).arg(filePath);
				return(false);
			}
		}
	}
	else
	{
		out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(i + 1).arg(filePath);
		return(false);
	}

	return(true);
}

/**
 * Parse a map
 *
 * @param i Line number
 * @param mapPath Path to the map file
 * @param lineItems List of the line items
 *
 * @return Status of parsing the map
 */
bool Parser::parseMap(int i, const QString &mapPath, const QStringList &lineItems)
{
	QTextStream out(stdout);

	bool status = false;
	QString name;
	QString tAddress;

	if(lineItems.size() == 3)
	{
		name = lineItems.at(0);
		tAddress = lineItems.at(2);

		status = this->parseMapCodeSection(i, mapPath, lineItems, name, tAddress);
	}
	else if(lineItems.size() == 5)
	{
		name = lineItems.at(2);
		tAddress = lineItems.at(4);

		status = this->parseMapMemory(i, mapPath, lineItems, name, tAddress);
	}

	if(!status)
	{
		out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(i + 1).arg(mapPath);
	}

	return(status);
}

/**
 * Parse a map code section
 *
 * @param i Line number
 * @param mapPath Path to the map file
 * @param lineItems List of the line items
 * @param name Name of entry
 * @param tAddress Address of entry in text form
 *
 * @return Status of parsing the map
 */
bool Parser::parseMapCodeSection(int i, const QString &mapPath, const QStringList &lineItems, QString &name, QString &tAddress)
{
	QTextStream out(stdout);
	unsigned int address;

	if((name.at(0) == ".") && (lineItems.at(1) == "="))
	{
		if(name.mid(1).contains(QRegularExpression("^([0-9a-zA-Z_-]+)$")))
		{
			if(tAddress.contains(QRegularExpression("^(0x[0-9abcdefABCDEF]+)$")))
			{
				bool tOk;
				address = tAddress.midRef(2).toUInt(&tOk, 16);

				if(tOk)
				{
					if(address < Data::MEMORY_SIZE)
					{
						QSharedPointer<CodeSection> codeSection(new CodeSection(name));
						codeSection->setAddress(address);

						// Add item as global one to not export it again to another map file
						if(!this->dataList.addGlobalCodeSection(codeSection))
						{
							out << QString("ERROR: Reuse of external section name: \"%1\", found at line %2 in file: \"%3\"\n").arg(name).arg(i + 1).arg(mapPath);
							return(false);
						}

						return(true);
					}
				}
			}
		}
	}

	return(false);
}

/**
 * Parse a map memory
 *
 * @param i Line number
 * @param mapPath Path to the map file
 * @param lineItems List of the line items
 * @param name Name of entry
 * @param tAddress Address of entry in text form
 *
 * @return Status of parsing the map
 */
bool Parser::parseMapMemory(int i, const QString &mapPath, const QStringList &lineItems, QString &name, QString &tAddress)
{
	QTextStream out(stdout);
	unsigned int address;

	if((!lineItems.at(0).compare("external", Qt::CaseInsensitive)) && (lineItems.at(3) == "="))
	{
		if(name.contains(QRegularExpression("^([0-9a-zA-Z_-]+)$")))
		{
			if(tAddress.contains(QRegularExpression("^(0x[0-9abcdefABCDEF]+)$")))
			{
				bool tOk;
				address = tAddress.midRef(2).toUInt(&tOk, 16);

				if(tOk)
				{
					if(address < Data::MEMORY_SIZE)
					{
						if(!lineItems.at(1).compare("var", Qt::CaseInsensitive))
						{
							QSharedPointer<RamItem> ramItem(new RamItem(0, name, ""));
							ramItem->setAddress(address);

							// Add item as global one to not export it again to another map file
							if(!this->dataList.addGlobalRamItem(ramItem))
							{
								out << QString("ERROR: Reuse of external section name: \"%1\", found at line %2 in file: \"%3\"\n").arg(name).arg(i + 1).arg(mapPath);
								return(false);
							}
						}
						else if(!lineItems.at(1).compare("const", Qt::CaseInsensitive))
						{
							QSharedPointer<FlashItem> flashItem(new FlashItem(0, name, QList<QString>()));
							flashItem->setAddress(address);

							// Add item as global one to not export it again to another map file
							if(!this->dataList.addGlobalFlashItem(flashItem))
							{
								out << QString("ERROR: Reuse of external section name: \"%1\", found at line %2 in file: \"%3\"\n").arg(name).arg(i + 1).arg(mapPath);
								return(false);
							}
						}

						return(true);
					}
				}
			}
		}
	}

	return(false);
}

/**
 * Parse a variable line
 *
 * @param i Line number
 * @param line Line raw data
 *
 * @return RAM Item object
 */
QSharedPointer<RamItem> Parser::parseRam(int i, const QString &line)
{
	QSharedPointer<RamItem> ramItem(nullptr);
	bool parseError = false;

	QString name;
	QString arraySize;

	if(line.length() == 0)
	{
		parseError = true;
	}

	if(line.contains(QRegularExpression("^([a-zA-Z0-9_-]+)$")))
	{
		name = line.trimmed();
	}
	else if(line.contains(QRegularExpression(R"(^([a-zA-Z0-9_-]+)\[([0-9]+)\]$)")))
	{
		int offset = line.indexOf("[");

		name = line.left(offset).trimmed();
		arraySize = line.mid((offset + 1), (line.length() - offset - 2));
	}
	else
	{
		parseError = true;
	}

	if(!parseError)
	{
		ramItem.reset(new RamItem((i + 1), name, arraySize));
	}

	return(ramItem);
}

/**
 * Parse a const line
 *
 * @param i Line number
 * @param line Line raw data
 *
 * @return Flash Item object
 */
QSharedPointer<FlashItem> Parser::parseFlash(int i, const QString &line)
{
	QSharedPointer<FlashItem> flashItem(nullptr);
	bool parseError = false;

	QString name;
	QString tLine;
	QStringList valueList;

	int offset = line.indexOf(QRegularExpression("\\s+"));

	if(offset > 0)
	{
		name = line.left(offset).trimmed();
		tLine = line.mid(name.length()).trimmed();

		if(tLine.length() > 0)
		{
			valueList = tLine.split(QRegularExpression(R"((?<!\\),)"));
		}

		if(valueList.empty())
		{
			parseError = true;
		}
	}
	else
	{
		parseError = true;
	}

	if(!parseError)
	{
		for(int i = 0; i < valueList.size(); i++)
		{
			QString value = valueList.at(i);
			value = value.remove(QRegularExpression(R"((?<!\\)\\)")).trimmed();

			valueList.replace(i, value);

			if(valueList.at(i).length() == 0)
			{
				parseError = true;
				break;
			}
		}

		if(!parseError)
		{
			flashItem.reset(new FlashItem((i + 1), name, valueList));
		}
	}

	return(flashItem);
}

/**
 * Parse a code line
 *
 * @param i Line number
 * @param label Name of the label
 * @param line Line raw data
 *
 * @return Code Item object
 */
QSharedPointer<CodeItem> Parser::parseCode(int i, const QString &label, const QString &line)
{
	QSharedPointer<CodeItem> codeItem(nullptr);
	bool parseError = false;

	QStringList paramList;

	if(line.length() > 0)
	{
		paramList = line.split(QRegularExpression("(?<!\\\\),"));
	}

	for(int i = 0; i < paramList.length(); i++)
	{
		paramList.replace(i, paramList.at(i).trimmed());

		if(paramList.at(i).isEmpty())
		{
			parseError = true;
			break;
		}
	}

	if(!parseError)
	{
		for(int i = 0; i < paramList.length(); i++)
		{
			if(paramList.at(i).at(0) != "'")
			{
				paramList.replace(i, paramList.at(i));
			}
			else
			{
				paramList.replace(i, QString(paramList.at(i)).remove(QRegularExpression(R"((?<!\\)\\)")).trimmed());
			}
		}

		codeItem.reset(new CodeItem((i + 1), label, paramList));
	}

	return(codeItem);
}
