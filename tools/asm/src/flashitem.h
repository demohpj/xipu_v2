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

#ifndef FLASHITEM_H
#define FLASHITEM_H

#include <QString>
#include <QList>
#include <QVector>
#include <QHash>

//! This class contains a one initialized read-only data item stored in the flash
class FlashItem
{
	public:
		FlashItem(int line, const QString &name, const QList<QString> &valueList);

		int getLine() const;
		const QString &getName() const;

		const QList<QString> &getValueList() const;

		int getDataSize() const;
		QVector<unsigned char> &getDataVector();

		void setAddress(unsigned int address);
		unsigned int getAddress() const;

	private:
		int line; //!< Line number from the source file
		QString name; //!< Name of the data item

		QList<QString> valueList; //!< List of data values in raw text format

		QVector<unsigned char> dataVector;//!< List of bytes generated from the data declaration
		unsigned int address; //!< Address in memory after linking
};

#endif
