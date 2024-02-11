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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QList>

#include "step.h"

//! This class contains atomic steps for the control unit of defined instruction
class Instruction
{
	public:
		//! Definitions of the instruction argument types
		enum class Arg
		{
			None, //!< No argument
			ABXY, //!< Main and auxiliary registers
			AB, //!< Only main registers
			Val256, //!< 8 bit unsigned value
			Addr //!< 16 bit unsigned address
		};

		//! Tristate flag definition for carry and zero control bits
		enum class Flag
		{
			Disable, //!< Only accept the disable value
			Enable, //!< Only accept the enable value
			Any //!< Accept both values
		};

		Instruction();
		Instruction(unsigned char opcode, Arg arg0 = Arg::None, Arg arg1 = Arg::None, Arg arg2 = Arg::None, Flag c = Flag::Any, Flag z = Flag::Any);

		void addStep(const Step &step);

		unsigned char getOpcode() const;

		Arg getArg0() const;
		Arg getArg1() const;
		Arg getArg2() const;

		Flag getC() const;
		Flag getZ() const;

		const QList<Step> &getStepList() const;

	private:
		unsigned char opcode; //!< Opcode of defined instruction

		Arg arg0; //!< First argument type
		Arg arg1; //!< Second argument type
		Arg arg2; //!< Third argument type

		Flag c; //!< Carry flag accept status
		Flag z; //!< Zero flag accept status

		QList<Step> stepList; //!< Atomic list of steps for the control unit
};

#endif
