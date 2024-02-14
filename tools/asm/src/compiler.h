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

#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <QTextStream>
#include <QList>
#include <QRegularExpression>
#include <QVector>

#include "instruction.h"
#include "codeitem.h"
#include "codesection.h"
#include "ramitem.h"
#include "flashitem.h"
#include "data.h"
#include "datalist.h"

//! This class transforms a parsed code to the binary executable form
class Compiler
{
	public:
		Compiler(DataList &dataList);

		void addInstruction(const Instruction &instruction);
		bool compile();

	private:
		bool compileRam(Data &data);
		bool compileFlash(Data &data);
		bool compileCode(Data &data);

		bool compileInstruction(const Data &data, CodeItem &codeItem);
		bool compileSubInstruction(const Data &data, CodeItem &codeItem, const Instruction &instruction);

		QList<Instruction::ArgType> checkParamType(const QString &param);
		const Instruction *findInstruction(const CodeItem &codeItem);

		bool parseParamAB(const QString &param, unsigned char &value);
		bool parseParamABXY(const QString &param, unsigned char &value);
		bool parseParamVal256(const QString &param, unsigned char &value);
		bool parseParamAddr(const QString &param, unsigned char &valueLow, unsigned char &valueHigh);

		bool isAddrLabel(const QString &param) const;

		DataList &dataList; //!< List of data for each source file
		QList<Instruction> instructionList; //!< List of instructions supported by the CPU
};

#endif
