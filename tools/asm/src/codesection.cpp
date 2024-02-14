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

#include "codesection.h"

/**
 * Constructor for creating a code section
 *
 * @param name Name of the code section
 */
CodeSection::CodeSection(const QString &name)
{
	this->name = name;
	this->address = 0;
}

/**
 * Add a code item to the code section
 *
 * @param codeItem Code item
 */
void CodeSection::addCodeItem(const QSharedPointer<CodeItem> &codeItem)
{
	this->codeItemList.append(codeItem);
}

/**
 * Get the name of the code section
 *
 * @return Name of the code section
 */
const QString &CodeSection::getName() const
{
	return(this->name);
}

/**
 * Get the code item list
 *
 * @return Code item list
 */
QList<QSharedPointer<CodeItem>> &CodeSection::getCodeItemList()
{
	return(this->codeItemList);
}

/**
 * Set the address in the memory
 *
 * @param address Address in the memory
 */
void CodeSection::setAddress(unsigned int address)
{
	this->address = address;
}

/**
 * Get the address in the memory
 *
 * @return Address in the memory
 */
unsigned int CodeSection::getAddress() const
{
	return(this->address);
}
