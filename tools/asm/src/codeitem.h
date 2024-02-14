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

#ifndef CODEITEM_H
#define CODEITEM_H

#include <QString>
#include <QList>
#include <QVector>
#include <QHash>

//! This class contains a one assembler instruction
class CodeItem
{
	public:
		CodeItem(int line, const QString &name, const QList<QString> &paramList);

		int getLine() const;
		const QString &getName() const;

		const QList<QString> &getParamList() const;

		QVector<unsigned char> &getDataVector();
		QHash<QString, unsigned int> &getToFillUpHash();

		void setAddress(unsigned int address);
		unsigned int getAddress() const;

	private:
		int line; //!< Line number from the source file
		QString name; //!< Name of the assembler instruction

		QList<QString> paramList; //!< List of parameters of the assembler instruction in raw text format

		QVector<unsigned char> dataVector; //!< List of bytes generated from the assembler instruction to be a machine instruction
		QHash<QString, unsigned int> toFillUpHash; //!< List of address fields connected to label needed to fill up during linking
		unsigned int address; //!< Address in memory after linking
};

#endif
