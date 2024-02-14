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

#ifndef RAMITEM_H
#define RAMITEM_H

#include <QString>

//! This class contains a one non-initialized read-write data item stored in the ram
class RamItem
{
	public:
		RamItem(int line, const QString &name, const QString &arraySize);

		int getLine() const;
		const QString &getName() const;
		const QString &getArraySize() const;

		void setDataSize(unsigned int size);
		unsigned int getDataSize() const;

		void setAddress(unsigned int address);
		unsigned int getAddress() const;

	private:
		int line; //!< Line number from the source file
		QString name; //!< Name of the data item
		QString arraySize; //!< Data size in raw text format

		unsigned int dataSize; //!< Data size
		unsigned int address; //!< Address in memory after linking
};

#endif
