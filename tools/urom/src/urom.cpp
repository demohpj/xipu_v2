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

#include "urom.h"

/**
 * Generate a binary microcode data
 *
 * @return Status of the generate operation
 */
bool URom::generateData()
{
	bool status = true;

	status &= this->generateDataLogic();
	status &= this->generateDataArithmetic();
	status &= this->generateDataMove();
	status &= this->generateDataIO();
	status &= this->generateDataMemory();
	status &= this->generateDataJump();
	status &= this->generateDataStack();
	status &= this->generateDataLoop();
	status &= this->generateDataFunction();
	status &= this->generateDataFunctionFrame();
	status &= this->generateDataMisc();

	return(status);
}

/**
 * Save a binary microcode to files
 *
 * @param urom0Path Path to the first output file
 * @param urom1Path Path to the second output file
 *
 * @return Status of the save operation
 */
bool URom::saveFiles(const QString &urom0Path, const QString &urom1Path)
{
	// Try to save the data to the output files
	return(this->uRomData.saveFiles(urom0Path, urom1Path));
}

/**
 * Generate a binary microcode data for logic instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataLogic()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + ALU + LOAD + RPC+1 //

	// NOT reg
	instruction = Instruction(0b00100100, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// AND reg, reg
	instruction = Instruction(0b00101000, Instruction::Arg::AB, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b1011, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// OR reg, reg
	instruction = Instruction(0b00110000, Instruction::Arg::AB, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b1110, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// XOR reg, reg
	instruction = Instruction(0b00111000, Instruction::Arg::AB, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b0110, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// SHL reg
	instruction = Instruction(0b01001100, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + 8x ALU + LOAD + RPC+1 //

	// SHR reg
	instruction = Instruction(0b01010000, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b01010000, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + RPC+1 //

	// AND val, reg
	instruction = Instruction(0b01000000, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1011, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// OR val, reg
	instruction = Instruction(0b01000100, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1110, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// XOR val, reg
	instruction = Instruction(0b01001000, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, true, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for arithmetic instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataArithmetic()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + ALU + LOAD + RPC+1 //

	// INC reg
	instruction = Instruction(0b10000000, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// INCC reg
	instruction = Instruction(0b10000100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b10000100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// DEC reg
	instruction = Instruction(0b10001000, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1111, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// DECC reg
	instruction = Instruction(0b10001100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1111, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b10001100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1111, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// ADD reg, reg
	instruction = Instruction(0b10010000, Instruction::Arg::AB, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b1001, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// ADDC reg, reg
	instruction = Instruction(0b10011000, Instruction::Arg::AB, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b1001, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b10011000, Instruction::Arg::AB, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b1001, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// SUB reg, reg
	instruction = Instruction(0b10100000, Instruction::Arg::AB, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b0110, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// SUBC reg, reg
	instruction = Instruction(0b10101000, Instruction::Arg::AB, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b0110, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b10101000, Instruction::Arg::AB, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b0110, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + RPC+1 //

	// ADD val, reg
	instruction = Instruction(0b10110000, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// ADDC val, reg
	instruction = Instruction(0b10110100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b10110100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// SUB val, reg
	instruction = Instruction(0b10111000, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// SUBC val, reg
	instruction = Instruction(0b10111100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b10111100, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for move instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataMove()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + LOAD + RPC+1 //

	// MOV reg, reg
	instruction = Instruction(0b11010000, Instruction::Arg::ABXY, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD + RPC+1 //

	// MOV val, reg
	instruction = Instruction(0b11001100, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + ALU + LOAD + RPC+1 //

	// CLR reg
	instruction = Instruction(0b001010100, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0011, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + 8x ALU + LOAD + RPC+1 //

	// SWAP reg
	instruction = Instruction(0b01011000, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b01011000, Instruction::Arg::ABXY, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1100, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + LOAD_D + LOAD + RPC+1 + LOAD_I + LOAD + RPC+1 //

	// XCHG reg, reg
	instruction = Instruction(0b01100000, Instruction::Arg::ABXY, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b01011100, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_MAL + PC+1 + LOAD_MAH + 3x LOAD + RPC+1 //

	// XCHG addr, reg
	instruction = Instruction(0b00011000, Instruction::Arg::Addr, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for IO access instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataIO()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + LOAD + RPC+1 //

	// IN reg
	instruction = Instruction(0b11110000, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::IN, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// OUT reg
	instruction = Instruction(0b11110100, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::OUT));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD + RPC+1 //

	// OUT val
	instruction = Instruction(0b00000011, Instruction::Arg::Val256);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::OUT));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for memory access instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataMemory()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + PC+1 + LOAD_MAL + PC+1 + LOAD_MAH + LOAD + RPC+1 //

	// LD addr, reg
	instruction = Instruction(0b11100100, Instruction::Arg::Addr, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// ST reg, addr
	instruction = Instruction(0b11101100, Instruction::Arg::ABXY, Instruction::Arg::Addr);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D +  PC+1 + LOAD_MAL + PC+1 + LOAD_MAH + LOAD + RPC+1 //

	// ST val, addr
	instruction = Instruction(0b00000010, Instruction::Arg::Val256, Instruction::Arg::Addr);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::MA, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + LOAD + RPC+1 //

	// STP val
	instruction = Instruction(0b00000001, Instruction::Arg::Val256);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::XY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + LOAD + RPC+1 //

	// LDP reg
	instruction = Instruction(0b11100000, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY, Step::BusB::Default, Step::BusC::XY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// STP reg
	instruction = Instruction(0b11101000, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::XY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD + PC+1 + LOAD_D+  RPC+1 + LOAD_I + LOAD + RPC+1 //

	// GP addr
	instruction = Instruction(0b00001110, Instruction::Arg::Addr);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b00001111);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// ADDRESS IS SHARED BETWEEN LDPO AND STPO

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + RPC+1 //

	// LDPO_STPO_0
	instruction = Instruction(0b00001010, Instruction::Arg::Val256);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + ALU + LOAD_MAH + RPC+1 //

	// LDPO_STPO_1
	instruction = Instruction(0b00001011, Instruction::Arg::None, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Disable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	instruction = Instruction(0b00001011, Instruction::Arg::None, Instruction::Arg::None, Instruction::Arg::None, Instruction::Flag::Enable);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, true));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + LOAD + RPC+1 //

	// LDPO_2
	instruction = Instruction(0b00001000, Instruction::Arg::AB);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// STPO_2
	instruction = Instruction(0b00001100, Instruction::Arg::AB);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::MA));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for jump instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataJump()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + ALU + RPC+1 //

	// CMP reg, reg
	instruction = Instruction(0b11000000, Instruction::Arg::AB, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::AB, Step::BusC::Default, 0b0110, false, true));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + ALU + RPC+1 //

	// CMP val, reg
	instruction = Instruction(0b11001000, Instruction::Arg::Val256, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, true));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + RESET //

	// JMP addr
	instruction = Instruction(0b11111000, Instruction::Arg::Addr);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + RESET //
	//		  + PC+1 + PC+1 + RPC+1 //

	// JE addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b11111001, Instruction::Arg::Addr, Instruction::Arg::None, Instruction::Arg::None, cFlag, zFlag);

			if(c & z)
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
				instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			}
			else
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	// JNE addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b11111010, Instruction::Arg::Addr, Instruction::Arg::None, Instruction::Arg::None, cFlag, zFlag);

			if(!(c & z))
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
				instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			}
			else
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	// JG addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b11111011, Instruction::Arg::Addr, Instruction::Arg::None, Instruction::Arg::None, cFlag, zFlag);

			if(c & !z)
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
				instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			}
			else
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	// JGE addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b11111100, Instruction::Arg::Addr, Instruction::Arg::None, Instruction::Arg::None, cFlag, zFlag);

			if(c)
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
				instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			}
			else
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	// JL addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b11111101, Instruction::Arg::Addr, Instruction::Arg::None, Instruction::Arg::None, cFlag, zFlag);

			if(!c & !z)
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
				instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			}
			else
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	// JLE addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b11111110, Instruction::Arg::Addr, Instruction::Arg::None, Instruction::Arg::None, cFlag, zFlag);

			if((!c & !z) || (c & z))
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
				instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			}
			else
			{
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	return(status);
}

/**
 * Generate a binary microcode data for stack instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataStack()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + ALU + LOAD_SPL + LOAD_SPH + RPC+1 //

	// RSTSP
	instruction = Instruction(0b00000100);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0011, false, false));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + LOAD + SP+1 + RPC+1 //

	// PUSH reg
	instruction = Instruction(0b01111000, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_PLUS));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + SP-1 + LOAD + RPC+1 //

	// POP reg
	instruction = Instruction(0b01111100, Instruction::Arg::ABXY);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY, Step::BusB::Default, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + LOAD + SP+1 + RPC+1 //

	// PUSH val
	instruction = Instruction(0b00100001, Instruction::Arg::Val256);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_PLUS));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for loop instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataLoop()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + PC+1 + LOAD_D + ALU + PC+1 + PC+1 + RPC+1 //
	//                              + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + ALU + LOAD + RESET //

	// LOOPE val, reg, addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b00010000, Instruction::Arg::Val256, Instruction::Arg::ABXY, Instruction::Arg::Addr, cFlag, zFlag);
			instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
			instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
			instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
			instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, true));

			if(c & z)
			{
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}
			else
			{

				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
			}

			instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
			instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
			instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0000, false, false));
			instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
			instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
			status &= this->uRomData.addInstruction(instruction);
		}
	}

	// LOAD_I + ALU + LOAD_D + ALU + PC+1 + PC+1 + RPC+1 //
	//                             + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + ALU + LOAD + RESET //

	// LOOPZ reg, addr
	for(int c = 0; c < 2; c++)
	{
		for(int z = 0; z < 2; z++)
		{
			Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);
			Instruction::Flag zFlag = (z ? Instruction::Flag::Enable : Instruction::Flag::Disable);

			instruction = Instruction(0b00010100, Instruction::Arg::ABXY, Instruction::Arg::Addr, Instruction::Arg::None, cFlag, zFlag);
			instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
			instruction.addStep(Step(Step::BusAR::D, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b0011, false, false));
			instruction.addStep(Step(Step::BusAR::T, Step::BusAW::D));
			instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, true));

			if(c & z)
			{
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			}
			else
			{

				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
				instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
				instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
			}

			instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
			instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
			instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::ALU_T, Step::BusB::Default, Step::BusC::Default, 0b1111, false, true));
			instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));
			instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));

			status &= this->uRomData.addInstruction(instruction);
		}
	}

	return(status);
}

/**
 * Generate a binary microcode data for function instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataFunction()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + PC+1 + LOAD_D + PC+1 + LOAD + SP+1 + LOAD + SP+1 + LOAD_PCH + LOAD_PCL + RESET //

	// CALL addr
	instruction = Instruction(0b00000101, Instruction::Arg::Addr);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
	instruction.addStep(Step(Step::BusAR::PC, Step::BusAW::RAM, Step::BusB::Low, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_PLUS));
	instruction.addStep(Step(Step::BusAR::PC, Step::BusAW::RAM, Step::BusB::High, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_PLUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::PC, Step::BusB::Low));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + SP-1 + LOAD_PCH + SP-1 + LOAD_PCL + RPC+1 //

	// RET
	instruction = Instruction(0b00000110);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::Low, Step::BusC::SP));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + PC+1 + LOAD_D + SP-1 + LOAD_PCH + SP-1 + LOAD_PCL + ALU + LOAD_SPL + ALU + LOAD_SPH + RPC+1 //

	// RET val
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00011110, Instruction::Arg::Val256, Instruction::Arg::None, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::High, Step::BusC::SP));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::PC, Step::BusB::Low, Step::BusC::SP));
		instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, false));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b1111, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b1111, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	return(status);
}

/**
 * Generate a binary microcode data for function frame instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataFunctionFrame()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + LOAD_SPL + SP+1 + LOAD_SPH + SP+1 + LOAD_D + ALU + LOAD_BPL + ALU + LOAD_BPH + RPC+1 //

	// ENTER
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00011111, Instruction::Arg::None, Instruction::Arg::None, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::RAM, Step::BusB::Low, Step::BusC::SP));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_PLUS));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::RAM, Step::BusB::High, Step::BusC::SP));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::BP, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b1111, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b1111, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::BP, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + LOAD_D + ALU + LOAD_SPL + ALU + LOAD_SPH + SP-1 + LOAD_BPH + SP-1 + LOAD_BPL + RPC+1 //

	// LEAVE
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00100000, Instruction::Arg::None, Instruction::Arg::None, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::BP, Step::BusB::High, Step::BusC::SP));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::SP_MINUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::BP, Step::BusB::Low, Step::BusC::SP));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_SPL + ALU + LOAD_SPH + RPC+1 //

	// ALLOC val
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00011100, Instruction::Arg::Val256, Instruction::Arg::None, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_SPL + ALU + LOAD_SPH + RPC+1 //

	// FREE val
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00011101, Instruction::Arg::Val256, Instruction::Arg::None, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b0110, false, false));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b1111, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::SP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b1111, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::SP, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + ALU + LOAD_MAH + LOAD + RPC+1 //

	// LDF val, reg
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b01110000, Instruction::Arg::Val256, Instruction::Arg::ABXY, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::ABXY, Step::BusB::Default, Step::BusC::MA));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + ALU + LOAD_MAH + LOAD + RPC+1 //

	// STF reg, val
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b01110100, Instruction::Arg::ABXY, Instruction::Arg::Val256, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::ABXY, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::MA));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + ALU + LOAD_MAH + PC+1 + LOAD_D + LOAD + RPC+1 //

	// STF val, val
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00000111, Instruction::Arg::Val256, Instruction::Arg::Val256, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::Low));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::MA, Step::BusB::High));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::D, Step::BusAW::RAM, Step::BusB::Default, Step::BusC::MA));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + ALU + LOAD_D + RPC+1 + LOAD_I + LOAD + RPC+1 //

	// GPF var
	for(int c = 0; c < 2; c++)
	{
		Instruction::Flag cFlag = (c ? Instruction::Flag::Enable : Instruction::Flag::Disable);

		instruction = Instruction(0b00100010, Instruction::Arg::Val256, Instruction::Arg::None, Instruction::Arg::None, cFlag);
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::PC_PLUS));
		instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::D, Step::BusC::Default, 0b1001, false, true));
		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::ABXY));

		if(c)
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, true));
		}
		else
		{
			instruction.addStep(Step(Step::BusAR::BP, Step::BusAW::ALU_T, Step::BusB::High, Step::BusC::Default, 0b0000, false, false));
		}

		instruction.addStep(Step(Step::BusAR::T, Step::BusAW::D));
		instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
		status &= this->uRomData.addInstruction(instruction);
	}

	instruction = Instruction(0b00100011);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::D, Step::BusAW::ABXY));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
	status &= this->uRomData.addInstruction(instruction);

	return(status);
}

/**
 * Generate a binary microcode data for misc instructions
 *
 * @return Status of the generate operation
 */
bool URom::generateDataMisc()
{
	bool status = true;
	Instruction instruction;

	// LOAD_I + RPC //

	// HALT
	instruction = Instruction(0b11111111);
	instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
	instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC));
	status &= this->uRomData.addInstruction(instruction);

	// LOAD_I + RPC+1 //

	// NOP
	// Fill all unused opcodes as NOP
	for(unsigned int i = 0; i < URomData::INSTRUCTION_QUANTITY; i++)
	{
		if(!this->uRomData.isOpcodeSet(i))
		{
			instruction = Instruction(static_cast<unsigned char>(i));
			instruction.addStep(Step(Step::BusAR::RAM, Step::BusAW::I));
			instruction.addStep(Step(Step::BusAR::Default, Step::BusAW::RPC_PLUS));
			status &= this->uRomData.addInstruction(instruction);
		}
	}

	return(status);
}
