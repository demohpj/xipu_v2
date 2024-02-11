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

#include "uromdata.h"

//! Default constructor for creating an empty uROM opcode table
URomData::URomData()
{
	this->data[0].fill(0);
	this->data[1].fill(0);

	this->opcodeIsSet.fill(false);
	this->dataIsSet.fill(false);
}

/**
 * Add an instruction to the uROM opcode table
 *
 * @param instruction Instruction to add
 *
 * @return Status of the add operation
 */
bool URomData::addInstruction(const Instruction &instruction)
{
	int opcodeWidth = 1;

	// Calculate how many consecutive opcodes the instruction uses
	opcodeWidth *= ((instruction.getArg0() == Instruction::Arg::AB) ? 2 : 1);
	opcodeWidth *= ((instruction.getArg0() == Instruction::Arg::ABXY) ? 4 : 1);

	opcodeWidth *= ((instruction.getArg1() == Instruction::Arg::AB) ? 2 : 1);
	opcodeWidth *= ((instruction.getArg1() == Instruction::Arg::ABXY) ? 4 : 1);

	// Calculate how many steps is used by the instruction
	int stepQuantity = qMin(instruction.getStepList().size(), INSTRUCTION_CYCLE_QUANTITY);

	// Add instruction entrys to the opcode table
	for(int i = 0; i < opcodeWidth; i++)
	{
		unsigned char opcode = (instruction.getOpcode() + static_cast<unsigned char>(i));

		this->opcodeIsSet[opcode] = true;

		for(int s = 0; s < stepQuantity; s++)
		{
			// Calculate the base offset of the entry
			int offsetBase = ((static_cast<int>(s) * INSTRUCTION_QUANTITY) + static_cast<int>(instruction.getOpcode()) + i);
			Step step = instruction.getStepList().at(s);

			// Fill only entries where the carry and the zero flags fit to the selected by instruction
			for(int c = 0; c < 2; c++)
			{
				for(int z = 0; z < 2; z++)
				{
					if(!((instruction.getC() == Instruction::Flag::Any) || (static_cast<int>(instruction.getC()) == c)))
					{
						continue;
					}

					if(!((instruction.getZ() == Instruction::Flag::Any) || (static_cast<int>(instruction.getZ()) == z)))
					{
						continue;
					}

					int offset = (offsetBase + (c << ADDRESS_FLAG_C_POSITION) + (z << ADDRESS_FLAG_Z_POSITION));

					// Return error when entry contains any data
					if(this->dataIsSet[offset])
					{
						QTextStream out(stdout);

						out << QString("ERROR: Duplicated usage of uROM data entry for opcode 0x%1 in step %2\n").arg(instruction.getOpcode(), 2, 16, QChar('0')).arg(s);
						return(false);
					}

					this->data[0][offset] = step.getUCode().getCode0();
					this->data[1][offset] = step.getUCode().getCode1();

					this->dataIsSet[offset] = true;
				}
			}
		}
	}

	return(true);
}

/**
 * Check if the opcode is set
 *
 * @param opcode Opcode number
 *
 * @return Status of set the opcode
 */
bool URomData::isOpcodeSet(unsigned char opcode) const
{
	return(this->opcodeIsSet[opcode]);
}

/**
 * Save a binary microcode to files
 *
 * @param urom0Path Path to the first output file
 * @param urom1Path Path to the second output file
 *
 * @return Status of the save operation
 */
bool URomData::saveFiles(const QString &urom0Path, const QString &urom1Path)
{
	QFile urom0File(urom0Path);

	// Try to save the first output file
	if(urom0File.open(QIODevice::WriteOnly))
	{
		urom0File.write(reinterpret_cast<const char *>(this->data[0].constData()), UROM_SIZE);
		urom0File.close();
	}
	else
	{
		return(false);
	}

	QFile urom1File(urom1Path);

	// Try to save the second output file
	if(urom1File.open(QIODevice::WriteOnly))
	{
		urom1File.write(reinterpret_cast<const char *>(this->data[1].constData()), UROM_SIZE);
		urom1File.close();
	}
	else
	{
		return(false);
	}

	return(true);
}
