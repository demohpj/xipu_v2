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

#include "datalist.h"

/**
 * Constructor for creating a data list
 *
 * @param mode Mode selection for how the application should be compiled and where in memory it will reside
 */
DataList::DataList(DataList::Mode mode)
{
	this->mode = mode;
}

/**
 * Get the mode
 *
 * @return Mode
 */
DataList::Mode DataList::getMode() const
{
	return(this->mode);
}

/**
 * Add an external code section to the data list
 *
 * @param codeSection External code section
 *
 * @return Status of adding the item
 */
bool DataList::addExternalCodeSection(const QSharedPointer<CodeSection> &codeSection)
{
	if(this->addGlobalCodeSection(codeSection))
	{
		this->externalCodeSectionList.append(codeSection);

		return(true);
	}

	return(false);
}

/**
 * Add an external Flash item to the data list
 *
 * @param flashItem External Flash item
 *
 * @return Status of adding the item
 */
bool DataList::addExternalFlashItem(const QSharedPointer<FlashItem> &flashItem)
{
	if(this->addGlobalFlashItem(flashItem))
	{
		this->externalFlashItemList.append(flashItem);

		return(true);
	}

	return(false);
}

/**
 * Add an external RAM item to the data list
 *
 * @param ramItem External RAM item
 *
 * @return Status of adding the item
 */
bool DataList::addExternalRamItem(const QSharedPointer<RamItem> &ramItem)
{
	if(this->addGlobalRamItem(ramItem))
	{
		this->externalRamItemList.append(ramItem);

		return(true);
	}

	return(false);
}

/**
 * Add a global code section to the data list
 *
 * @param codeSection Global code section
 *
 * @return Status of adding the item
 */
bool DataList::addGlobalCodeSection(const QSharedPointer<CodeSection> &codeSection)
{
	for(const QSharedPointer<CodeSection> &codeSectionFor : qAsConst(this->globalCodeSectionList))
	{
		if(!codeSectionFor->getName().compare(codeSection->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	this->globalCodeSectionList.append(codeSection);

	return(true);
}

/**
 * Add a global Flash item to the data list
 *
 * @param flashItem Global Flash item
 *
 * @return Status of adding the item
 */
bool DataList::addGlobalFlashItem(const QSharedPointer<FlashItem> &flashItem)
{
	for(const QSharedPointer<FlashItem> &flashItemFor : qAsConst(this->globalFlashItemList))
	{
		if(!flashItemFor->getName().compare(flashItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	for(const QSharedPointer<RamItem> &ramItemFor : qAsConst(this->globalRamItemList))
	{
		if(!ramItemFor->getName().compare(flashItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	this->globalFlashItemList.append(flashItem);

	return(true);
}

/**
 * Add a global RAM item to the data list
 *
 * @param ramItem Global RAM item
 *
 * @return Status of adding the item
 */
bool DataList::addGlobalRamItem(const QSharedPointer<RamItem> &ramItem)
{
	for(const QSharedPointer<RamItem> &ramItemFor : qAsConst(this->globalRamItemList))
	{
		if(!ramItemFor->getName().compare(ramItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	for(const QSharedPointer<FlashItem> &flashItemFor : qAsConst(this->globalFlashItemList))
	{
		if(!flashItemFor->getName().compare(ramItem->getName(), Qt::CaseInsensitive))
		{
			return(false);
		}
	}

	this->globalRamItemList.append(ramItem);

	return(true);
}

/**
 * Get the external code section list
 *
 * @return External code section list
 */
QList<QSharedPointer<CodeSection>> &DataList::getExternalCodeSectionList()
{
	return(this->externalCodeSectionList);
}

/**
 * Get the external Flash item list
 *
 * @return External Flash item list
 */
QList<QSharedPointer<FlashItem>> &DataList::getExternalFlashItemList()
{
	return(this->externalFlashItemList);
}

/**
 * Get the external RAM item list
 *
 * @return External RAM item list
 */
QList<QSharedPointer<RamItem>> &DataList::getExternalRamItemList()
{
	return(this->externalRamItemList);
}

/**
 * Get the global code section list
 *
 * @return Global code section list
 */
QList<QSharedPointer<CodeSection>> &DataList::getGlobalCodeSectionList()
{
	return(this->globalCodeSectionList);
}

/**
 * Get the global Flash item list
 *
 * @return Global Flash item list
 */
QList<QSharedPointer<FlashItem>> &DataList::getGlobalFlashItemList()
{
	return(this->globalFlashItemList);
}

/**
 * Get the global RAM item list
 *
 * @return Global RAM item list
 */
QList<QSharedPointer<RamItem>> &DataList::getGlobalRamItemList()
{
	return(this->globalRamItemList);
}
