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

#include "data.h"

/**
 * Constructor for creating a data
 *
 * @param fileName Name of the source file
 */
Data::Data(const QString &fileName)
{
	this->fileName = fileName;
}

/**
 * Get the source file name
 *
 * @return Source file name
 */
const QString &Data::getFileName() const
{
	return(this->fileName);
}

/**
 * Add a code section to the data
 *
 * @param codeSection Code section
 *
 * @return Status of adding the item
 */
bool Data::addCodeSection(const QSharedPointer<CodeSection> &codeSection)
{
	for(const QSharedPointer<CodeSection> &codeSectionFor : qAsConst(this->codeSectionList))
	{
		if(!codeSectionFor->getName().compare(codeSection->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	this->codeSectionList.append(codeSection);

	return(true);
}

/**
 * Add a Flash item to the data
 *
 * @param flashItem Flash item
 *
 * @return Status of adding the item
 */
bool Data::addFlashItem(const QSharedPointer<FlashItem> &flashItem)
{
	for(const QSharedPointer<FlashItem> &flashItemFor : qAsConst(this->flashItemList))
	{
		if(!flashItemFor->getName().compare(flashItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	for(const QSharedPointer<RamItem> &ramItemFor : qAsConst(this->ramItemList))
	{
		if(!ramItemFor->getName().compare(flashItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	this->flashItemList.append(flashItem);

	return(true);
}

/**
 * Add a RAM item to the data
 *
 * @param ramItem RAM item
 *
 * @return Status of adding the item
 */
bool Data::addRamItem(const QSharedPointer<RamItem> &ramItem)
{
	for(const QSharedPointer<RamItem> &ramItemFor : qAsConst(this->ramItemList))
	{
		if(!ramItemFor->getName().compare(ramItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	for(const QSharedPointer<FlashItem> &flashItemFor : qAsConst(this->flashItemList))
	{
		if(!flashItemFor->getName().compare(ramItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	this->ramItemList.append(ramItem);

	return(true);
}

/**
 * Get the code section list
 *
 * @return Code section list
 */
QList<QSharedPointer<CodeSection>> &Data::getCodeSectionList()
{
	return(this->codeSectionList);
}

/**
 * Get the Flash item list
 *
 * @return Flash item list
 */
QList<QSharedPointer<FlashItem>> &Data::getFlashItemList()
{
	return(this->flashItemList);
}

/**
 * Get the RAM item list
 *
 * @return RAM item list
 */
QList<QSharedPointer<RamItem>> &Data::getRamItemList()
{
	return(this->ramItemList);
}
