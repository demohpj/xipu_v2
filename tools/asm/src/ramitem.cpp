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

#include "ramitem.h"

/**
 * Constructor for creating a RAM item
 *
 * @param line Line number in the source file
 * @param name Name of the variable
 * @param arraySize Array size in a raw form
 */
RamItem::RamItem(int line, const QString &name, const QString &arraySize)
{
	this->line = line;
	this->name = name;
	this->arraySize = arraySize;

	this->dataSize = 0;
	this->address = 0;
}

/**
 * Get the line number where the variable occurred
 *
 * @return Line number in the source file
 */
int RamItem::getLine() const
{
	return(this->line);
}

/**
 * Get the name of the variable
 *
 * @return Name of the variable
 */
const QString &RamItem::getName() const
{
	return(this->name);
}

/**
 * Get the variable size in a raw form
 *
 * @return Variable size in a raw form
 */
const QString &RamItem::getArraySize() const
{
	return(this->arraySize);
}

/**
 * Set the size of the variable
 *
 * @param size Size of the variable
 */
void RamItem::setDataSize(unsigned int size)
{
	this->dataSize = size;
}

/**
 * Get the size of the variable
 *
 * @return Size of the variable
 */
unsigned int RamItem::getDataSize() const
{
	return(this->dataSize);
}

/**
 * Set the address in the memory
 *
 * @param address Address in the memory
 */
void RamItem::setAddress(unsigned int address)
{
	this->address = address;
}

/**
 * Get the address in the memory
 *
 * @return Address in the memory
 */
unsigned int RamItem::getAddress() const
{
	return(this->address);
}
