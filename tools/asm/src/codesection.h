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

#ifndef CODESECTION_H
#define CODESECTION_H

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "codeitem.h"

//! This class contains a one code section set by each label
class CodeSection
{
	public:
		CodeSection(const QString &name);

		void addCodeItem(const QSharedPointer<CodeItem> &codeItem);

		const QString &getName() const;
		QList<QSharedPointer<CodeItem>> &getCodeItemList();

		void setAddress(unsigned int address);
		unsigned int getAddress() const;

	private:
		QString name; //!< Label name of the code section

		QList<QSharedPointer<CodeItem>> codeItemList; //!< Instruction list attached to the code section
		unsigned int address; //!< Address in the memory of the code section
};

#endif
