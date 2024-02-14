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

#include <QString>
#include <QList>

//! This class contains a one description of the CPU instruction
class Instruction
{
	public:
		//! Definitions of the instruction argument
		enum class Arg
		{
			None, //!< None argument. It could be used to make a void argument in a sub instruction
			ABXY_0, //!< Master ABXY register field in opcode
			ABXY_1, //!< Auxiliary ABXY register field in opcode
			ABXY_V, //!< Virtual ABXY register. It could be used to make a room for an argument to be used in a sub instruction
			AB_0, //!< Master AB register field in opcode
			AB_1, //!< Auxiliary AB register field in opcode
			AB_V, //!< Virtual AB register. It could be used to make a room for an argument to be used in a sub instruction
			Val256_0, //!< First payload for an instruction. It contains Val256
			Val256_1, //!< Second payload for an instruction. It contains Val256
			Val256_V, //!< Virtual payload for an instruction. It contains Val256. It could be used to make a room for an argument to be used in a sub instruction
			Addr_0, //!< First payload for an instruction. It contains Addr
			Addr_1, //!< Second payload for an instruction. It contains Addr
			Addr_V //!< Virtual payload for an instruction. It contains Addr. It could be used to make a room for an argument to be used in a sub instruction
		};

		//! Definitions of the instruction argument types
		enum class ArgType
		{
			ABXY, //!< Main and auxiliary registers
			AB, //!< Only main registers
			Val256, //!< 8 bit unsigned value
			Addr //!< 16 bit unsigned address
		};

		Instruction(const QString &name, unsigned char opcode, Arg arg0 = Arg::None, Arg arg1 = Arg::None, Arg arg2 = Arg::None);

		const QString &getName() const;
		unsigned char getOpcode() const;
		const QList<Arg> &getArgList() const;
		const QList<ArgType> &getArgTypeList() const;

		Instruction &appendSubInstruction(unsigned char opcode, Arg arg0 = Arg::None, Arg arg1 = Arg::None, Arg arg2 = Arg::None);
		const QList<Instruction> &getSubInstructionList() const;

	private:
		QString name; //!< Instruction name
		unsigned char opcode; //!< Opcode of the defined instruction
		QList<Arg> argList; //!< Argument list of the defined instruction
		QList<ArgType> argTypeList; //!< Argument type list of the defined instruction

		QList<Instruction> subInstructionList; //!< Sub instruction list

		void addArg(Arg arg);
};

#endif
