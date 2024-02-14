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

#include "instruction.h"

/**
 * Constructor using for create an instruction with a defined opcode
 *
 * @param name Name of the instruction
 * @param opcode Opcode number
 * @param arg0 First argument
 * @param arg1 Second argument
 * @param arg2 Third argument
 */
Instruction::Instruction(const QString &name, unsigned char opcode, Instruction::Arg arg0, Instruction::Arg arg1, Instruction::Arg arg2)
{
	this->name = name;
	this->opcode = opcode;

	this->addArg(arg0);
	this->addArg(arg1);
	this->addArg(arg2);
}

/**
 * Get the name of the instruction
 *
 * @return Name of the instruction
 */
const QString &Instruction::getName() const
{
	return(this->name);
}

/**
 * Get the opcode number of the instruction
 *
 * @return Opcode of the instruction
 */
unsigned char Instruction::getOpcode() const
{
	return(this->opcode);
}

/**
 * Get the argument list of the instruction
 *
 * @return Argument list
 */
const QList<Instruction::Arg> &Instruction::getArgList() const
{
	return(this->argList);
}

/**
 * Get the argument type list of the instruction
 *
 * @return Argument type list
 */
const QList<Instruction::ArgType> &Instruction::getArgTypeList() const
{
	return(this->argTypeList);
}

/**
 * Append a new sub instruction
 *
 * @param opcode Opcode number
 * @param arg0 First argument
 * @param arg1 Second argument
 * @param arg2 Third argument
 *
 * @return Reference to this object
 */
Instruction &Instruction::appendSubInstruction(unsigned char opcode, Arg arg0, Arg arg1, Arg arg2)
{
	this->subInstructionList.append(Instruction("", opcode, arg0, arg1, arg2));

	return(*this);
}

/**
 * Get the sub instruction list
 *
 * @return Sub instruction list
 */
const QList<Instruction> &Instruction::getSubInstructionList() const
{
	return(this->subInstructionList);
}

/**
 * Append a new argument
 *
 * @param arg Argument
 */
void Instruction::addArg(Arg arg)
{
	switch(arg)
	{
		case Arg::ABXY_0 :
		case Arg::ABXY_1 :
		case Arg::ABXY_V :
			this->argTypeList.append(ArgType::ABXY);
			break;

		case Arg::AB_0 :
		case Arg::AB_1 :
		case Arg::AB_V :
			this->argTypeList.append(ArgType::AB);
			break;

		case Arg::Val256_0 :
		case Arg::Val256_1 :
		case Arg::Val256_V :
			this->argTypeList.append(ArgType::Val256);
			break;

		case Arg::Addr_0 :
		case Arg::Addr_1 :
		case Arg::Addr_V :
			this->argTypeList.append(ArgType::Addr);
			break;

		default :
			break;
	}

	this->argList.append(arg);
}
