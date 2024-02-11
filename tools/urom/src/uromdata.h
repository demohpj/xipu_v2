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

#ifndef UROMDATA_H
#define UROMDATA_H

#include <QString>
#include <QFile>
#include <QVector>
#include <QTextStream>

#include "instruction.h"

//! This class contains a whole generated microcode for all instructions
class URomData
{
	public:
		static const int INSTRUCTION_QUANTITY = 256; //!< Support only for 256 opcodes
		static const int INSTRUCTION_CYCLE_QUANTITY = 16; //!< Every instruction can get only 16 steps

		static const int ADDRESS_FLAG_C_POSITION = 12; //!< Position of the carry bit received from ALU to the control unit
		static const int ADDRESS_FLAG_Z_POSITION = 13; //!< Position of the zero bit received from ALU to the control unit

		static const int UROM_SIZE = 32768; //!< uROM size for low and high banks

		URomData();

		bool addInstruction(const Instruction &instruction);
		bool isOpcodeSet(unsigned char opcode) const;
		bool saveFiles(const QString &urom0Path, const QString &urom1Path);

	private:
		//! Data container for low and high banks
		QVector<QVector<unsigned char>> data = { QVector<unsigned char>(UROM_SIZE), QVector<unsigned char>(UROM_SIZE) };

		QVector<bool> opcodeIsSet = QVector<bool>(INSTRUCTION_QUANTITY); //!< It is set when a selected opcode number is defined
		QVector<bool> dataIsSet = QVector<bool>(UROM_SIZE); //!< It is set when a selected data entry is used
};

#endif
