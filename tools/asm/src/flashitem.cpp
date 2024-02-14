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

#include "flashitem.h"

/**
 * Constructor for creating a Flash item
 *
 * @param line Line number in the source file
 * @param name Name of the const data
 * @param valueList List of const data which will be stored in the Flash
 */
FlashItem::FlashItem(int line, const QString &name, const QList<QString> &valueList)
{
	this->line = line;
	this->name = name;

	this->valueList = valueList;
	this->address = 0;
}

/**
 * Get the line number where the const data occurred
 *
 * @return Line number in the source file
 */
int FlashItem::getLine() const
{
	return(this->line);
}

/**
 * Get the name of the const data
 *
 * @return Name of the const data
 */
const QString &FlashItem::getName() const
{
	return(this->name);
}

/**
 * Get the list of the const data in a raw form
 *
 * @return List of the const data in a raw form
 */
const QList<QString> &FlashItem::getValueList() const
{
	return(this->valueList);
}

/**
 * Get the size of the const data
 *
 * @return Size of the const data
 */
int FlashItem::getDataSize() const
{
	return(this->dataVector.size());
}

/**
 * Get the list of the const data in a binary form
 *
 * @return List of the const data in a binary form
 */
QVector<unsigned char> &FlashItem::getDataVector()
{
	return(this->dataVector);
}

/**
 * Set the address in the memory
 *
 * @param address Address in the memory
 */
void FlashItem::setAddress(unsigned int address)
{
	this->address = address;
}

/**
 * Get the address in the memory
 *
 * @return Address in the memory
 */
unsigned int FlashItem::getAddress() const
{
	return(this->address);
}
