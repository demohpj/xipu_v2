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

#include "asm.h"

/**
 * Compile the BIOS
 *
 * @param inputFile Name of the input source file
 * @param outputFile Name of the output binary file
 * @param outputMapFile Name of the output map file
 *
 * @return Status of compilation
 */
bool Asm::compileBios(const QString &inputFile, const QString &outputFile, const QString &outputMapFile)
{
	return(this->compile(DataList::Mode::Bios, inputFile, outputFile, outputMapFile));
}

/**
 * Compile the OS
 *
 * @param inputFile Name of the input source file
 * @param outputFile Name of the output binary file
 * @param outputMapFile Name of the output map file
 *
 * @return Status of compilation
 */
bool Asm::compileOs(const QString &inputFile, const QString &outputFile, const QString &outputMapFile)
{
	return(this->compile(DataList::Mode::Os, inputFile, outputFile, outputMapFile));
}

/**
 * Compile the application
 *
 * @param inputFile Name of the input source file
 * @param outputFile Name of the output binary file
 *
 * @return Status of compilation
 */
bool Asm::compileApp(const QString &inputFile, const QString &outputFile)
{
	return(this->compile(DataList::Mode::App, inputFile, outputFile));
}

/**
 * Compile the source
 *
 * @param mode Mode of compilation
 * @param inputFile Name of the input source file
 * @param outputFile Name of the output binary file
 * @param outputMapFile Name of the output map file
 *
 * @return Status of compilation
 */
bool Asm::compile(DataList::Mode mode, const QString &inputFile, const QString &outputFile, const QString &outputMapFile)
{
	QTextStream out(stdout);

	DataList dataList(mode);
	Parser parser(dataList);

	if(!parser.parseFile(inputFile))
	{
		out << "ERROR: Parsing error\n\n";
		return(false);
	}

	Compiler compiler(dataList);
	this->initInstructions(compiler);

	if(!compiler.compile())
	{
		out << "ERROR: Compilation error\n\n";
		return(false);
	}

	Linker linker(dataList);

	if(!linker.link())
	{
		out << "ERROR: Linking error\n\n";
		return(false);
	}

	if(!linker.saveFile(outputFile, outputMapFile))
	{
		out << "ERROR: Failed to save file\n\n";
		return(false);
	}

	out << "OK: The file was saved successfully\n\n";
	return(true);
}

/**
 * Init the instruction list for the compiler
 *
 * @param compiler Compiler to which the instructions will be added
 */
void Asm::initInstructions(Compiler &compiler)
{
	// LOGIC INSTRUCTIONS

	// LOAD_I + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("not", 0b00100100, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("and", 0b00101000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("or", 0b00110000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("xor", 0b00111000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("shl", 0b01001100, Instruction::Arg::ABXY_0));

	// LOAD_I + 8x ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("shr", 0b01010000, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("and", 0b01000000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("or", 0b01000100, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("xor", 0b01001000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));

	// ARITHMETIC INSTRUCTIONS

	// LOAD_I + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("inc", 0b10000000, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("incc", 0b10000100, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("dec", 0b10001000, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("decc", 0b10001100, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("add", 0b10010000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("addc", 0b10011000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("sub", 0b10100000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("subc", 0b10101000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("add", 0b10110000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("addc", 0b10110100, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("sub", 0b10111000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("subc", 0b10111100, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));

	// VIRTUAL ARITHMETIC INSTRUCTIONS

	// LOAD_I + ALU + LOAD + RPC+1 + LOAD_I + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("incp", 0b10000010).appendSubInstruction(0b10000111));
	compiler.addInstruction(Instruction("decp", 0b10001010).appendSubInstruction(0b10001111));
	compiler.addInstruction(Instruction("addp", 0b10010010, Instruction::Arg::AB_1).appendSubInstruction(0b10000111));
	compiler.addInstruction(Instruction("subp", 0b10100010, Instruction::Arg::AB_1).appendSubInstruction(0b10001111));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + RPC+1 + LOAD_I + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("addp", 0b10110010, Instruction::Arg::Val256_0).appendSubInstruction(0b10000111));
	compiler.addInstruction(Instruction("subp", 0b10111010, Instruction::Arg::Val256_0).appendSubInstruction(0b10001111));

	// MOVE INSTRUCTIONS

	// LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("mov", 0b11010000, Instruction::Arg::ABXY_1, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD + RPC+1
	compiler.addInstruction(Instruction("mov", 0b11001100, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));

	// LOAD_I + ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("clr", 0b01010100, Instruction::Arg::ABXY_0));

	// LOAD_I + 8x ALU + LOAD + RPC+1
	compiler.addInstruction(Instruction("swap", 0b01011000, Instruction::Arg::ABXY_0));

	// LOAD_I + LOAD_D + LOAD + RPC+1 + LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("xchg", 0b01100000, Instruction::Arg::ABXY_1, Instruction::Arg::ABXY_0).appendSubInstruction(0b01011100, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_MAL + PC+1 + LOAD_MAH + 3x LOAD + RPC+1
	compiler.addInstruction(Instruction("xchg", 0b00011000, Instruction::Arg::Addr_0, Instruction::Arg::ABXY_0));

	// IO INSTRUCTIONS

	// LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("in", 0b11110000, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("out", 0b11110100, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD + RPC+1
	compiler.addInstruction(Instruction("out", 0b00000011, Instruction::Arg::Val256_0));

	// MEMORY INSTRUCTIONS

	// LOAD_I + PC+1 + LOAD_MAL + PC+1 + LOAD_MAH + LOAD + RPC+1
	compiler.addInstruction(Instruction("ld", 0b11100100, Instruction::Arg::Addr_0, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("st", 0b11101100, Instruction::Arg::ABXY_0, Instruction::Arg::Addr_0));

	// LOAD_I + PC+1 + LOAD_D +  PC+1 + LOAD_MAL + PC+1 + LOAD_MAH + LOAD + RPC+1
	compiler.addInstruction(Instruction("st", 0b00000010, Instruction::Arg::Val256_1, Instruction::Arg::Addr_0));

	// LOAD_I + PC+1 + LOAD_D + LOAD + RPC+1
	compiler.addInstruction(Instruction("stp", 0b00000001, Instruction::Arg::Val256_0));

	// LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("ldp", 0b11100000, Instruction::Arg::ABXY_0));
	compiler.addInstruction(Instruction("stp", 0b11101000, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD + PC+1 + LOAD_D+  RPC+1 + LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("gp", 0b00001110, Instruction::Arg::Addr_0).appendSubInstruction(0b00001111));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + RPC+1 + LOAD_I + ALU + LOAD_MAH + RPC+1 + LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("ldpo", 0b00001010, Instruction::Arg::Val256_0, Instruction::Arg::AB_V).appendSubInstruction(0b00001011).appendSubInstruction(0b00001000, Instruction::Arg::None, Instruction::Arg::AB_0));
	compiler.addInstruction(Instruction("stpo", 0b00001010, Instruction::Arg::AB_V, Instruction::Arg::Val256_0).appendSubInstruction(0b00001011).appendSubInstruction(0b00001100, Instruction::Arg::AB_0));

	// JUMP INSTRUCTIONS

	// LOAD_I + ALU + RPC+1
	compiler.addInstruction(Instruction("cmp", 0b11000000, Instruction::Arg::AB_1, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + RPC+1
	compiler.addInstruction(Instruction("cmp", 0b11001000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + RESET
	compiler.addInstruction(Instruction("jmp", 0b11111000, Instruction::Arg::Addr_0));

	// LOAD_I + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + RESET
	//		  + PC+1 + PC+1 + RPC+1
	compiler.addInstruction(Instruction("je", 0b11111001, Instruction::Arg::Addr_0));
	compiler.addInstruction(Instruction("jne", 0b11111010, Instruction::Arg::Addr_0));
	compiler.addInstruction(Instruction("jg", 0b11111011, Instruction::Arg::Addr_0));
	compiler.addInstruction(Instruction("jge", 0b11111100, Instruction::Arg::Addr_0));
	compiler.addInstruction(Instruction("jl", 0b11111101, Instruction::Arg::Addr_0));
	compiler.addInstruction(Instruction("jle", 0b11111110, Instruction::Arg::Addr_0));

	// STACK INSTRUCTIONS

	// LOAD_I + ALU + LOAD_SPL + LOAD_SPH + RPC+1
	compiler.addInstruction(Instruction("rstsp", 0b00000100));

	// LOAD_I + LOAD + SP+1 + RPC+1
	compiler.addInstruction(Instruction("push", 0b01111000, Instruction::Arg::ABXY_0));

	// LOAD_I + SP-1 + LOAD + RPC+1
	compiler.addInstruction(Instruction("pop", 0b01111100, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_D + LOAD + SP+1 + RPC+1
	compiler.addInstruction(Instruction("push", 0b00100001, Instruction::Arg::Val256_0));

	// LOOP INSTRUCTIONS

	// LOAD_I + PC+1 + LOAD_D + ALU + PC+1 + PC+1 + RPC+1
	//                              + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + ALU + LOAD + RESET
	compiler.addInstruction(Instruction("loope", 0b00010000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0, Instruction::Arg::Addr_1));

	// LOAD_I + ALU + LOAD_D + ALU + PC+1 + PC+1 + RPC+1
	//                             + PC+1 + LOAD_D + PC+1 + LOAD_PCH + LOAD_PCL + ALU + LOAD + RESET
	compiler.addInstruction(Instruction("loopz", 0b00010100, Instruction::Arg::ABXY_0, Instruction::Arg::Addr_0));

	// FUNCTION INSTRUCTIONS

	// LOAD_I + PC+1 + LOAD_D + PC+1 + LOAD + SP+1 + LOAD + SP+1 + LOAD_PCH + LOAD_PCL + RESET
	compiler.addInstruction(Instruction("call", 0b00000101, Instruction::Arg::Addr_0));

	// LOAD_I + SP-1 + LOAD_PCH + SP-1 + LOAD_PCL + RPC+1
	compiler.addInstruction(Instruction("ret", 0b00000110));

	// LOAD_I + PC+1 + LOAD_D + SP-1 + LOAD_PCH + SP-1 + LOAD_PCL + ALU + LOAD_SPL + ALU + LOAD_SPH + RPC+1
	compiler.addInstruction(Instruction("ret", 0b00011110, Instruction::Arg::Val256_0));

	// FUNCTION FRAME INSTRUCTIONS

	// LOAD_I + LOAD_SPL + SP+1 + LOAD_SPH + SP+1 + LOAD_D + ALU + LOAD_BPL + ALU + LOAD_BPH + RPC+1
	compiler.addInstruction(Instruction("enter", 0b00011111));

	// LOAD_I + LOAD_D + ALU + LOAD_SPL + ALU + LOAD_SPH + SP-1 + LOAD_BPH + SP-1 + LOAD_BPL + RPC+1
	compiler.addInstruction(Instruction("leave", 0b00100000));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_SPL + ALU + LOAD_SPH + RPC+1
	compiler.addInstruction(Instruction("alloc", 0b00011100, Instruction::Arg::Val256_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_SPL + ALU + LOAD_SPH + RPC+1
	compiler.addInstruction(Instruction("free", 0b00011101, Instruction::Arg::Val256_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + ALU + LOAD_MAH + LOAD + RPC+1
	compiler.addInstruction(Instruction("ldf", 0b01110000, Instruction::Arg::Val256_0, Instruction::Arg::ABXY_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + ALU + LOAD_MAH + LOAD + RPC+1
	compiler.addInstruction(Instruction("stf", 0b01110100, Instruction::Arg::ABXY_0, Instruction::Arg::Val256_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD_MAL + ALU + LOAD_MAH + PC+1 + LOAD_D + LOAD + RPC+1
	compiler.addInstruction(Instruction("stf", 0b00000111, Instruction::Arg::Val256_1, Instruction::Arg::Val256_0));

	// LOAD_I + PC+1 + LOAD_D + ALU + LOAD + ALU + LOAD_D + RPC+1 + LOAD_I + LOAD + RPC+1
	compiler.addInstruction(Instruction("gpf", 0b00100010, Instruction::Arg::Val256_0).appendSubInstruction(0b00100011));

	// MISC INSTRUCTIONS

	// LOAD_I + RPC
	compiler.addInstruction(Instruction("halt", 0b11111111));

	// LOAD_I + RPC+1
	compiler.addInstruction(Instruction("nop", 0b00000000));
}
