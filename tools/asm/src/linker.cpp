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

#include "linker.h"

/**
 * Constructor of the linker
 *
 * @param dataList Data list
 */
Linker::Linker(DataList &dataList) : dataList(dataList)
{
}

/**
 * Link all parts of the application together and fill addresses
 *
 * @return Status of linking the application
 */
bool Linker::link()
{
	QTextStream out(stdout);

	unsigned int ramSize = 0;
	unsigned int ramOffset = 0;
	unsigned int ramUsage = 0;

	switch(this->dataList.getMode())
	{
		case DataList::Mode::Bios :
			ramSize = BIOS_RAM_SIZE;
			ramOffset = BIOS_RAM_OFFSET;
			break;

		case DataList::Mode::Os :
			ramSize = OS_RAM_SIZE;
			ramOffset = OS_RAM_OFFSET;
			break;

		default :
			ramSize = APP_RAM_SIZE;
			ramOffset = APP_RAM_OFFSET;
			break;
	}

	this->flashVector.clear();

	// Move code to flash
	for(const QSharedPointer<Data> &data : qAsConst(this->dataList))
	{
		// Copy code to flash list
		for(const QSharedPointer<CodeSection> &codeSection : qAsConst(data->getCodeSectionList()))
		{
			codeSection->setAddress(ramUsage + ramOffset);

			for(const QSharedPointer<CodeItem> &codeItem : qAsConst(codeSection->getCodeItemList()))
			{
				codeItem->setAddress(ramUsage + ramOffset);

				ramUsage += static_cast<unsigned int>(codeItem->getDataVector().length());

				this->flashVector.append(codeItem->getDataVector());
			}
		}
	}

	// Move data flash
	for(const QSharedPointer<Data> &data : qAsConst(this->dataList))
	{
		// Copy flash to flash list
		for(const QSharedPointer<FlashItem> &flashItem : qAsConst(data->getFlashItemList()))
		{
			flashItem->setAddress(ramUsage + ramOffset);

			ramUsage += static_cast<unsigned int>(flashItem->getDataSize());

			this->flashVector.append(flashItem->getDataVector());
		}
	}

	// Move data to ram and flash
	for(const QSharedPointer<Data> &data : qAsConst(this->dataList))
	{
		// Count ram
		for(const QSharedPointer<RamItem> &ramItem : qAsConst(data->getRamItemList()))
		{
			ramItem->setAddress(ramUsage + ramOffset);

			ramUsage += ramItem->getDataSize();
		}
	}

	if(ramUsage > ramSize)
	{
		out << QString("ERROR: %1 of %2 (%3%) bytes of RAM are used\n").arg(ramUsage).arg(ramSize).arg(((static_cast<double>(ramUsage) / static_cast<double>(ramSize)) * 100.0), 0, 'f', 1);
		return(false);
	}

	// Update addresses
	for(const QSharedPointer<Data> &data : qAsConst(this->dataList))
	{
		for(const QSharedPointer<CodeSection> &codeSection : qAsConst(data->getCodeSectionList()))
		{
			for(const QSharedPointer<CodeItem> &codeItem : qAsConst(codeSection->getCodeItemList()))
			{
				QHashIterator<QString, unsigned int> it(codeItem->getToFillUpHash());

				while(it.hasNext())
				{
					it.next();

					const QString &label = it.key();
					unsigned int offset = it.value();

					unsigned int address;

					offset += (codeItem->getAddress() - ramOffset);

					bool isOk = false;

					if((label.at(0) == ".") || (label.at(0) == "*"))
					{
						if(this->findAddress(*data, label, address))
						{
							this->flashVector.replace(static_cast<int>(offset), static_cast<unsigned char>(address % 256));
							this->flashVector.replace(static_cast<int>(offset + 1), static_cast<unsigned char>(address / 256));

							isOk = true;
						}
					}

					if(!isOk)
					{
						out << QString("ERROR: Unknown label name \"%1\" at line %2 in file: \"%3\"\n").arg(label).arg(codeItem->getLine()).arg(data->getFileName());
						return(false);
					}
				}
			}
		}
	}

	out << QString("INFO: %1 of %2 (%3%) bytes of RAM are used\n").arg(ramUsage).arg(ramSize).arg(((static_cast<double>(ramUsage) / static_cast<double>(ramSize)) * 100.0), 0, 'f', 1);

	return(true);
}

/**
 * Save the binary to the file
 *
 * @param path Path to the binary file
 * @param pathMap Path to the map file
 *
 * @return Status of saving data to the file
 */
bool Linker::saveFile(const QString &path, const QString &pathMap)
{
	if(!this->flashVector.empty())
	{
		// Fill the BIOS file with zeros if needed
		if(this->dataList.getMode() == DataList::Mode::Bios)
		{
			this->flashVector.append(QVector<unsigned char>((BIOS_RAM_SIZE - this->flashVector.size()), 0));
		}

		QFile file(path);

		if(file.open(QIODevice::WriteOnly))
		{
			file.write(reinterpret_cast<const char *>(this->flashVector.constData()), this->flashVector.size());
			file.close();
		}
		else
		{
			return(false);
		}
	}
	else
	{
		return(false);
	}

	if((this->dataList.getMode() == DataList::Mode::Bios) || (this->dataList.getMode() == DataList::Mode::Os))
	{
		QFile file(pathMap);

		if(file.open(QIODevice::WriteOnly))
		{
			for(const QSharedPointer<RamItem> &ramItem : qAsConst(this->dataList.getExternalRamItemList()))
			{
				QString line = QString("external var %1 = 0x%2\n").arg(ramItem->getName()).arg(ramItem->getAddress(), 4, 16, QChar('0'));

				file.write(line.toUtf8());
			}

			for(const QSharedPointer<FlashItem> &flashItem : qAsConst(this->dataList.getExternalFlashItemList()))
			{
				QString line = QString("external const %1 = 0x%2\n").arg(flashItem->getName()).arg(flashItem->getAddress(), 4, 16, QChar('0'));

				file.write(line.toUtf8());
			}

			for(const QSharedPointer<CodeSection> &codeSection : qAsConst(this->dataList.getExternalCodeSectionList()))
			{
				QString line = QString("%1 = 0x%2\n").arg(codeSection->getName()).arg(codeSection->getAddress(), 4, 16, QChar('0'));

				file.write(line.toUtf8());
			}

			file.close();
		}
		else
		{
			return(false);
		}
	}

	return(true);
}

/**
 * Find the correct address in memory
 *
 * @param data Data of the compiled application
 * @param label Label name to find
 * @param address Found address
 *
 * @return Status of searching the address
 */
bool Linker::findAddress(Data &data, const QString &label, unsigned int &address)
{
	QString name;
	bool status = false;

	if(label.contains(QRegularExpression(R"(^(\.[0-9a-zA-Z_-]+)$)")))
	{
		name = label;
		status = this->findCodeAddress(data, name, address);
	}
	else if(label.contains(QRegularExpression(R"(^(\*[0-9a-zA-Z_-]+)$)")))
	{
		name = label.mid(1);
		status = this->findMemoryAddress(data, name, address);
	}
	else if(label.contains(QRegularExpression(R"(^(\*[0-9a-zA-Z_-]+\[[0-9]+\])$)")))
	{
		int cOffset = label.indexOf("[");
		name = label.mid(1, (cOffset - 1));
		unsigned int offset = label.midRef((cOffset + 1), (label.length() - cOffset - 2)).toUInt();

		if(this->findMemoryAddress(data, name, address))
		{
			address += offset;
			status = true;
		}
	}

	return(status);
}

/**
 * Find the correct address of code in memory
 *
 * @param data Data of the compiled application
 * @param name Name to find
 * @param address Found address
 *
 * @return Status of searching the address
 */
bool Linker::findCodeAddress(Data &data, const QString &name, unsigned int &address)
{
	for(const QSharedPointer<CodeSection> &codeSection : qAsConst(data.getCodeSectionList()))
	{
		if(!codeSection->getName().compare(name, Qt::CaseInsensitive))
		{
			address = codeSection->getAddress();

			return(true);
		}
	}

	for(const QSharedPointer<CodeSection> &codeSection : qAsConst(this->dataList.getGlobalCodeSectionList()))
	{
		if(!codeSection->getName().compare(name, Qt::CaseInsensitive))
		{
			address = codeSection->getAddress();

			return(true);
		}
	}

	return(false);
}

/**
 * Find the correct address of variable or const in memory
 *
 * @param data Data of the compiled application
 * @param name Name to find
 * @param address Found address
 *
 * @return Status of searching the address
 */
bool Linker::findMemoryAddress(Data &data, const QString &name, unsigned int &address)
{
	for(const QSharedPointer<RamItem> &ramItem : qAsConst(data.getRamItemList()))
	{
		if(!ramItem->getName().compare(name, Qt::CaseInsensitive))
		{
			address = ramItem->getAddress();

			return(true);
		}
	}

	for(const QSharedPointer<FlashItem> &flashItem : qAsConst(data.getFlashItemList()))
	{
		if(!flashItem->getName().compare(name, Qt::CaseInsensitive))
		{
			address = flashItem->getAddress();

			return(true);
		}
	}

	for(const QSharedPointer<RamItem> &ramItem : qAsConst(this->dataList.getGlobalRamItemList()))
	{
		if(!ramItem->getName().compare(name, Qt::CaseInsensitive))
		{
			address = ramItem->getAddress();

			return(true);
		}
	}

	for(const QSharedPointer<FlashItem> &flashItem : qAsConst(this->dataList.getGlobalFlashItemList()))
	{
		if(!flashItem->getName().compare(name, Qt::CaseInsensitive))
		{
			address = flashItem->getAddress();

			return(true);
		}
	}

	return(false);
}
