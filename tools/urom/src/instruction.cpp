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

//! Default constructor for creating an empty instruction
Instruction::Instruction()
{
	this->opcode = 0;

	this->arg0 = Arg::None;
	this->arg1 = Arg::None;
	this->arg2 = Arg::None;

	this->c = Flag::Any;
	this->z = Flag::Any;
}

/**
 * Constructor using for create an instruction with a defined opcode
 *
 * @param opcode Opcode number
 * @param arg0 First argument type
 * @param arg1 Second argument type
 * @param arg2 Third argument type
 * @param c Which status of the carry flag is accepted
 * @param z Which status of the zero flag is accepted
 */
Instruction::Instruction(unsigned char opcode, Arg arg0, Arg arg1, Arg arg2, Flag c, Flag z)
{
	this->opcode = opcode;

	this->arg0 = arg0;
	this->arg1 = arg1;
	this->arg2 = arg2;

	this->c = c;
	this->z = z;
}

/**
 * Add step to the step list of the instruction
 *
 * @param step Single step of the instruction
 */
void Instruction::addStep(const Step &step)
{
	this->stepList.append(step);
}

/**
 * Get an opcode number of the instruction
 *
 * @return Opcode of the instruction
 */
unsigned char Instruction::getOpcode() const
{
	return(this->opcode);
}

/**
 * Get a first argument type of the instruction
 *
 * @return Argument type
 */
Instruction::Arg Instruction::getArg0() const
{
	return(this->arg0);
}

/**
 * Get a second argument type of the instruction
 *
 * @return Argument type
 */
Instruction::Arg Instruction::getArg1() const
{
	return(this->arg1);
}

/**
 * Get a third argument type of the instruction
 *
 * @return Argument type
 */
Instruction::Arg Instruction::getArg2() const
{
	return(this->arg2);
}

/**
 * Get a status of accepting the carry flag
 *
 * @return Accepting status of the carry flag
 */
Instruction::Flag Instruction::getC() const
{
	return(this->c);
}

/**
 * Get a status of accepting the zero flag
 *
 * @return Accepting status of the zero flag
 */
Instruction::Flag Instruction::getZ() const
{
	return(this->z);
}

/**
 * Get a step list of the instruction
 *
 * @return Step list of the instruction
 */
const QList<Step> &Instruction::getStepList() const
{
	return(this->stepList);
}
