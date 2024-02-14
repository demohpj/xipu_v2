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

#ifndef DATALIST_H
#define DATALIST_H

#include <QList>
#include <QSharedPointer>

#include "codesection.h"
#include "ramitem.h"
#include "flashitem.h"
#include "data.h"

/**
 * This class contains all parsed data for each source file.
 * It allows in a simply way to collect all external or global sections of code, RAM items and Flash items.
 * It contains too a selection of which mode is used to compile the application.
 */
class DataList : public QList<QSharedPointer<Data>>
{
	public:
		//! List of possible compile modes
		enum class Mode
		{
			Bios, //!< BIOS generate mode. It cannot use the RAM defined variables. It generates *.map file.
			Os, //!< OS generate mode. Used to generate a kernel file. It generates *.map file. It can use *.map files.
			App //!< User application generate mode. It can use *.map files.
		};

		DataList(Mode mode);

		Mode getMode() const;

		bool addExternalCodeSection(const QSharedPointer<CodeSection> &codeSection);
		bool addExternalFlashItem(const QSharedPointer<FlashItem> &flashItem);
		bool addExternalRamItem(const QSharedPointer<RamItem> &ramItem);

		bool addGlobalCodeSection(const QSharedPointer<CodeSection> &codeSection);
		bool addGlobalFlashItem(const QSharedPointer<FlashItem> &flashItem);
		bool addGlobalRamItem(const QSharedPointer<RamItem> &ramItem);

		QList<QSharedPointer<CodeSection>> &getExternalCodeSectionList();
		QList<QSharedPointer<FlashItem>> &getExternalFlashItemList();
		QList<QSharedPointer<RamItem>> &getExternalRamItemList();

		QList<QSharedPointer<CodeSection>> &getGlobalCodeSectionList();
		QList<QSharedPointer<FlashItem>> &getGlobalFlashItemList();
		QList<QSharedPointer<RamItem>> &getGlobalRamItemList();

	private:
		Mode mode; //!< Which mode is used to compilation the code

		QList<QSharedPointer<CodeSection>> externalCodeSectionList; //!< List of external code sections
		QList<QSharedPointer<FlashItem>> externalFlashItemList; //!< List of external Flash items
		QList<QSharedPointer<RamItem>> externalRamItemList; //!< List of external RAM items

		QList<QSharedPointer<CodeSection>> globalCodeSectionList; //!< List of global code section
		QList<QSharedPointer<FlashItem>> globalFlashItemList; //!< List of global Flash items
		QList<QSharedPointer<RamItem>> globalRamItemList; //!< List of global RAM item
};

#endif
