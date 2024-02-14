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

#include "codeitem.h"

/**
 * Constructor for creating a code item
 *
 * @param line Line number in the source file
 * @param name Name of the instruction
 * @param paramList List of parameters for the instruction
 */
CodeItem::CodeItem(int line, const QString &name, const QList<QString> &paramList)
{
	this->line = line;
	this->name = name;

	this->paramList = paramList;

	this->address = 0;
}

/**
 * Get the line number where the instruction occurred
 *
 * @return Line number in the source file
 */
int CodeItem::getLine() const
{
	return(this->line);
}

/**
 * Get the name of the instruction
 *
 * @return Name of the instruction
 */
const QString &CodeItem::getName() const
{
	return(this->name);
}

/**
 * Get the parameter list of the instruction
 *
 * @return Parameter list of the instruction
 */
const QList<QString> &CodeItem::getParamList() const
{
	return(this->paramList);
}

/**
 * Get the list of machine instructions generated from the assembler instruction
 *
 * @return List of machine instruction
 */
QVector<unsigned char> &CodeItem::getDataVector()
{
	return(this->dataVector);
}

/**
 * Get the list of address needed to fill up during linking
 *
 * @return List of address needed to fill up during linking
 */
QHash<QString, unsigned int> &CodeItem::getToFillUpHash()
{
	return(this->toFillUpHash);
}

/**
 * Set the address in the memory
 *
 * @param address Address in the memory
 */
void CodeItem::setAddress(unsigned int address)
{
	this->address = address;
}

/**
 * Get the address in the memory
 *
 * @return Address in the memory
 */
unsigned int CodeItem::getAddress() const
{
	return(this->address);
}
