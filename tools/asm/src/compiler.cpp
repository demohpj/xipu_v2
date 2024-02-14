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

#include "compiler.h"

/**
 * Constructor of the compiler
 *
 * @param dataList Data list
 */
Compiler::Compiler(DataList &dataList) : dataList(dataList)
{
}

/**
 * Add an instruction to the uROM opcode table
 *
 * @param instruction Instruction to add
 */
void Compiler::addInstruction(const Instruction &instruction)
{
	this->instructionList.append(instruction);
}

/**
 * Compile the application
 *
 * @return Status of compiling the application
 */
bool Compiler::compile()
{
	for(const QSharedPointer<Data> &data : qAsConst(this->dataList))
	{
		if(!(this->compileRam(*data)))
		{
			return(false);
		}

		if(!(this->compileFlash(*data)))
		{
			return(false);
		}

		if(!(this->compileCode(*data)))
		{
			return(false);
		}
	}

	return(true);
}

/**
 * Compile the RAM items
 *
 * @param data Data of the parsed application
 *
 * @return Status of compiling the RAM items
 */
bool Compiler::compileRam(Data &data)
{
	QTextStream out(stdout);

	for(const QSharedPointer<RamItem> &ramItem : qAsConst(data.getRamItemList()))
	{
		bool compiled = false;
		QString arraySize = ramItem->getArraySize();

		if(arraySize.length() > 0)
		{
			if(arraySize.contains(QRegularExpression("^([0-9]+)$")))
			{
				bool isOk;
				unsigned int size = arraySize.toUInt(&isOk, 10);

				if(isOk)
				{
					ramItem->setDataSize(size);
					compiled = true;
				}
			}
		}
		else
		{
			ramItem->setDataSize(1);
			compiled = true;
		}

		if(!compiled)
		{
			out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(ramItem->getLine()).arg(data.getFileName());
			return(false);
		}
	}

	return(true);
}

/**
 * Compile the Flash items
 *
 * @param data Data of the parsed application
 *
 * @return Status of compiling the Flash items
 */
bool Compiler::compileFlash(Data &data)
{
	QTextStream out(stdout);

	for(const QSharedPointer<FlashItem> &flashItem : qAsConst(data.getFlashItemList()))
	{
		bool compiled = true;
		bool isString = false;

		if(flashItem->getValueList().size() == 1)
		{
			QString string = flashItem->getValueList().at(0);

			if(string.contains(QRegularExpression("^(\".+\")$")))
			{
				isString = true;

				for(int j = 1; j < (string.length() - 1); j++)
				{
					char cChar = string.at(j).toLatin1();

					if(cChar > 0)
					{
						flashItem->getDataVector().append(static_cast<unsigned char>(cChar));
					}
					else
					{
						compiled = false;
						break;
					}
				}

				flashItem->getDataVector().append(0x00);
			}
		}

		if(!isString)
		{
			if(!flashItem->getValueList().empty())
			{
				for(int j = 0; j < flashItem->getValueList().size(); j++)
				{
					unsigned char value;

					if(this->parseParamVal256(flashItem->getValueList().at(j), value))
					{
						flashItem->getDataVector().append(value);
					}
					else
					{
						compiled = false;
						break;
					}
				}
			}
			else
			{
				compiled = false;
			}
		}

		if(!compiled)
		{
			out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(flashItem->getLine()).arg(data.getFileName());
			return(false);
		}
	}

	return(true);
}

/**
 * Compile the code items
 *
 * @param data Data of the parsed application
 *
 * @return Status of compiling the code items
 */
bool Compiler::compileCode(Data &data)
{
	for(const QSharedPointer<CodeSection> &codeSection : qAsConst(data.getCodeSectionList()))
	{
		for(const QSharedPointer<CodeItem> &codeItem : qAsConst(codeSection->getCodeItemList()))
		{
			if(!(this->compileInstruction(data, *codeItem)))
			{
				return(false);
			}
		}
	}

	return(true);
}

/**
 * Compile an instruction
 *
 * @param data Data of the parsed application
 * @param codeItem Code item to compile
 *
 * @return Status of compiling the instruction
 */
bool Compiler::compileInstruction(const Data &data, CodeItem &codeItem)
{
	QTextStream out(stdout);
	const Instruction *instruction = this->findInstruction(codeItem);

	if(instruction == nullptr)
	{
		out << QString("ERROR: Unknow instruction at line %1 in file: \"%2\"\n").arg(codeItem.getLine()).arg(data.getFileName());
		return(false);
	}

	unsigned char opCode = instruction->getOpcode();
	int argQuantity = instruction->getArgList().length();
	QVector<unsigned char> tmp(2);
	QVector<unsigned char> dataList;

	// Process the first position parameters
	for(int i = 0; i < argQuantity; i++)
	{
		bool isOk = true;
		tmp.fill(0);

		switch(instruction->getArgList().at(i))
		{
			case Instruction::Arg::AB_0 :
				isOk &= this->parseParamAB(codeItem.getParamList().at(i), tmp[0]);
				opCode |= tmp[0];
				break;

			case Instruction::Arg::ABXY_0 :
				isOk &= this->parseParamABXY(codeItem.getParamList().at(i), tmp[0]);
				opCode |= tmp[0];
				break;

			case Instruction::Arg::Val256_0 :
				isOk &= this->parseParamVal256(codeItem.getParamList().at(i), tmp[0]);
				dataList.append(tmp[0]);
				break;

			case Instruction::Arg::Addr_0 :
				if(isOk && this->isAddrLabel(codeItem.getParamList().at(i)))
				{
					codeItem.getToFillUpHash().insert(codeItem.getParamList().at(i), static_cast<unsigned int>(dataList.size() + 1));

					tmp[0] = 0x00;
					tmp[1] = 0x00;
				}
				else
				{
					isOk &= this->parseParamAddr(codeItem.getParamList().at(i), tmp[0], tmp[1]);
				}

				dataList.append(tmp[0]);
				dataList.append(tmp[1]);
				break;

			default :
				break;
		}

		if(!isOk)
		{
			out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(codeItem.getLine()).arg(data.getFileName());
			return(false);
		}
	}

	// Process the second position parameters
	for(int i = 0; i < argQuantity; i++)
	{
		bool isOk = true;
		tmp.fill(0);

		switch(instruction->getArgList().at(i))
		{
			case Instruction::Arg::AB_1 :
				isOk &= this->parseParamAB(codeItem.getParamList().at(i), tmp[0]);
				opCode |= (tmp[0] << 2);
				break;

			case Instruction::Arg::ABXY_1 :
				isOk &= this->parseParamABXY(codeItem.getParamList().at(i), tmp[0]);
				opCode |= (tmp[0] << 2);
				break;

			case Instruction::Arg::Val256_1 :
				isOk &= this->parseParamVal256(codeItem.getParamList().at(i), tmp[0]);
				dataList.append(tmp[0]);
				break;

			case Instruction::Arg::Addr_1 :
				if(isOk && this->isAddrLabel(codeItem.getParamList().at(i)))
				{
					codeItem.getToFillUpHash().insert(codeItem.getParamList().at(i), static_cast<unsigned int>(dataList.size() + 1));

					tmp[0] = 0x00;
					tmp[1] = 0x00;
				}
				else
				{
					isOk &= this->parseParamAddr(codeItem.getParamList().at(i), tmp[0], tmp[1]);
				}

				dataList.append(tmp[0]);
				dataList.append(tmp[1]);
				break;

			default :
				break;
		}

		if(!isOk)
		{
			out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(codeItem.getLine()).arg(data.getFileName());
			return(false);
		}
	}

	codeItem.getDataVector().append(opCode);
	codeItem.getDataVector().append(dataList);

	for(const Instruction &subInstruction : qAsConst(instruction->getSubInstructionList()))
	{
		if(!(this->compileSubInstruction(data, codeItem, subInstruction)))
		{
			return(false);
		}
	}

	return(true);
}

/**
 * Compile a sub instruction
 *
 * @param data Data of the parsed application
 * @param codeItem Code item to compile
 * @param instruction Sub instruction
 *
 * @return Status of compiling the sub instruction
 */
bool Compiler::compileSubInstruction(const Data &data, CodeItem &codeItem, const Instruction &instruction)
{
	QTextStream out(stdout);
	unsigned char opCode = instruction.getOpcode();
	int argQuantity = instruction.getArgList().length();
	unsigned char tmp;

	// Process the first position parameters
	for(int i = 0; i < argQuantity; i++)
	{
		bool isOk = true;
		tmp = 0;

		switch(instruction.getArgList().at(i))
		{
			case Instruction::Arg::AB_0 :
				isOk &= this->parseParamAB(codeItem.getParamList().at(i), tmp);
				opCode |= tmp;
				break;

			case Instruction::Arg::ABXY_0 :
				isOk &= this->parseParamABXY(codeItem.getParamList().at(i), tmp);
				opCode |= tmp;
				break;

			default :
				break;
		}

		if(!isOk)
		{
			out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(codeItem.getLine()).arg(data.getFileName());
			return(false);
		}
	}

	// Process the second position parameters
	for(int i = 0; i < argQuantity; i++)
	{
		bool isOk = true;
		tmp = 0;

		switch(instruction.getArgList().at(i))
		{
			case Instruction::Arg::AB_1 :
				isOk &= this->parseParamAB(codeItem.getParamList().at(i), tmp);
				opCode |= (tmp << 2);
				break;

			case Instruction::Arg::ABXY_1 :
				isOk &= this->parseParamABXY(codeItem.getParamList().at(i), tmp);
				opCode |= (tmp << 2);
				break;

			default :
				break;
		}

		if(!isOk)
		{
			out << QString("ERROR: Syntax error at line %1 in file: \"%2\"\n").arg(codeItem.getLine()).arg(data.getFileName());
			return(false);
		}
	}

	codeItem.getDataVector().append(opCode);

	return(true);
}

/**
 * Check the type of the param
 *
 * @param param String to check
 *
 * @return Param type
 */
QList<Instruction::ArgType> Compiler::checkParamType(const QString &param)
{
	QList<Instruction::ArgType> argList;

	if(param.contains(QRegularExpression("^(a|b|x|y|A|B|X|Y)$")))
	{
		argList.append(Instruction::ArgType::ABXY);

		if(param.contains(QRegularExpression("^(a|b|A|B)$")))
		{
			argList.append(Instruction::ArgType::AB);
		}
	}
	else if(param.contains(QRegularExpression("^([0-9]+|0b[01]+|0x[0-9abcdefABCDEF]+|'.')$")))
	{
		argList.append(Instruction::ArgType::Val256);

		if(param.contains(QRegularExpression("^(0x[0-9abcdefABCDEF]+)$")))
		{
			argList.append(Instruction::ArgType::Addr);
		}
	}
	else if(param.contains(QRegularExpression(R"(^(\.[0-9a-zA-Z_-]+)$)")))
	{
		argList.append(Instruction::ArgType::Addr);
	}
	else if(param.contains(QRegularExpression(R"(^(\*[0-9a-zA-Z_-]+)$)")))
	{
		argList.append(Instruction::ArgType::Addr);
	}
	else if(param.contains(QRegularExpression(R"(^(\*[0-9a-zA-Z_-]+\[[0-9]+\])$)")))
	{
		argList.append(Instruction::ArgType::Addr);
	}

	return(argList);
}

/**
 * Find a matching instruction
 *
 * @param codeItem Code item
 *
 * @return Found instruction or nullptr if not
 */
const Instruction *Compiler::findInstruction(const CodeItem &codeItem)
{
	int argQuantity = codeItem.getParamList().length();

	for(const Instruction &instruction : qAsConst(this->instructionList))
	{
		if((!instruction.getName().compare(codeItem.getName(), Qt::CaseInsensitive)) && (instruction.getArgTypeList().length() == argQuantity))
		{
			bool found = true;

			for(int i = 0; i < argQuantity; i++)
			{
				QList<Instruction::ArgType> argType = this->checkParamType(codeItem.getParamList().at(i));

				found &= argType.contains(instruction.getArgTypeList().at(i));
			}

			if(found)
			{
				return(&instruction);
			}
		}
	}

	return(nullptr);
}

/**
 * Parse the string and return AB
 *
 * @param param String to parsing
 * @param value Parsed value
 *
 * @return Status of parsing the string
 */
bool Compiler::parseParamAB(const QString &param, unsigned char &value)
{
	if(!param.compare("a", Qt::CaseInsensitive))
	{
		value = 0;
	}
	else if(!param.compare("b", Qt::CaseInsensitive))
	{
		value = 1;
	}
	else
	{
		return(false);
	}

	return(true);
}

/**
 * Parse the string and return ABXY
 *
 * @param param String to parsing
 * @param value Parsed value
 *
 * @return Status of parsing the string
 */
bool Compiler::parseParamABXY(const QString &param, unsigned char &value)
{
	if(!param.compare("a", Qt::CaseInsensitive))
	{
		value = 0;
	}
	else if(!param.compare("b", Qt::CaseInsensitive))
	{
		value = 1;
	}
	else if(!param.compare("x", Qt::CaseInsensitive))
	{
		value = 2;
	}
	else if(!param.compare("y", Qt::CaseInsensitive))
	{
		value = 3;
	}
	else
	{
		return(false);
	}

	return(true);
}

/**
 * Parse the string and return Val256
 *
 * @param param String to parsing
 * @param value Parsed value
 *
 * @return Status of parsing the string
 */
bool Compiler::parseParamVal256(const QString &param, unsigned char &value)
{
	bool isOk = false;
	unsigned int number = 0;

	if(param.contains(QRegularExpression("^([0-9]+)$")))
	{
		number = param.toUInt(&isOk, 10);
	}
	else if(param.contains(QRegularExpression("^(0b[01]+)$")))
	{
		number = param.midRef(2).toUInt(&isOk, 2);
	}
	else if(param.contains(QRegularExpression("^(0x[0-9abcdefABCDEF]+)$")))
	{
		number = param.midRef(2).toUInt(&isOk, 16);
	}
	else if(param.contains(QRegularExpression("^('.')$")))
	{
		char cChar = param.at(1).toLatin1();

		if(cChar > 0)
		{
			number = static_cast<unsigned int>(cChar);
			isOk = true;
		}
	}

	if(isOk)
	{
		if(number < 256)
		{
			value = static_cast<unsigned char>(number);
		}
		else
		{
			isOk = false;
		}
	}

	return(isOk);
}

/**
 * Parse the string and return address
 *
 * @param param String to parsing
 * @param valueLow Parsed lower byte of the value
 * @param valueHigh Parsed higher byte of the value
 *
 * @return Status of parsing the string
 */
bool Compiler::parseParamAddr(const QString &param, unsigned char &valueLow, unsigned char &valueHigh)
{
	bool isOk = false;
	unsigned int number = 0;

	if(param.contains(QRegularExpression("^(0x[0-9abcdefABCDEF]+)$")))
	{
		number = param.midRef(2).toUInt(&isOk, 16);
	}

	if(isOk)
	{
		if(number < Data::MEMORY_SIZE)
		{
			valueLow = static_cast<unsigned char>(number % 256);
			valueHigh = static_cast<unsigned char>(number / 256);
		}
		else
		{
			isOk = false;
		}
	}

	return(isOk);
}

/**
 * Check if the param is an address label
 *
 * @param param String to check
 *
 * @return Status of the check
 */
bool Compiler::isAddrLabel(const QString &param) const
{
	bool status = false;

	if(param.contains(QRegularExpression(R"(^(\.[0-9a-zA-Z_-]+)$)")))
	{
		status = true;
	}
	else if(param.contains(QRegularExpression(R"(^(\*[0-9a-zA-Z_-]+)$)")))
	{
		status = true;
	}
	else if(param.contains(QRegularExpression(R"(^(\*[0-9a-zA-Z_-]+\[[0-9]+\])$)")))
	{
		status = true;
	}

	return(status);
}
