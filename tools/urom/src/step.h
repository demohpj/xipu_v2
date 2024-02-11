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

#ifndef STEP_H
#define STEP_H

#include <QVector>

#include "ucode.h"

/**
 * This class contains a single step of the instruction.
 * It is a raw describe how to set control lines for the step.
 */
class Step
{
	public:
		static const int CODE_0_BUS_AR_POSITION = 0; //!< Position of bits for the main reading data bus
		static const int CODE_0_BUS_AW_POSITION = 3; //!< Position of bits for the main writing data bus
		static const int CODE_0_BUS_B_POSITION = 7; //!< Position of bits for the secondary reading data bus

		static const int CODE_1_BUS_C_POSITION = 0; //!< Position of bits for the addressing data bus
		static const int CODE_1_FLAG_S_POSITION = 2; //!< Position of bits for the ALU_S flags
		static const int CODE_1_FLAG_M_POSITION = 6; //!< Position of bit for the ALU_M flag
		static const int CODE_1_FLAG_C_POSITION = 7; //!< Position of bit for the ALU_C flag

		static const unsigned char CODE_FLAG_S_MASK = 0x0f; //!< Mask for ALU_S flags

		//! Line select defines for the main reading data bus
		enum class BusAR
		{
			ABXY = 0, //!< Main and auxiliary registers
			D, //!< Hidden data register
			IN, //!< Input register
			T, //!< ALU temp register
			RAM, //!< RAM access
			PC, //!< Program Counter pair of registers
			SP, //!< Stack Pointer pair of registers
			BP, //!< Base Pointer pair of registers
			Default = 0
		};

		//! Line select defines for the main writing data bus
		enum class BusAW
		{
			None = 0, //!< Not selected
			ABXY, //!< Main and auxiliary registers
			D, //!< Hidden data register
			OUT, //!< Output register
			ALU_T, //!< ALU operation
			RPC, //!< Reset Program Counter
			I, //!< Instruction register
			RAM, //!< RAM access
			PC, //!< Program Counter pair of registers
			SP, //!< Stack Pointer pair of registers
			BP, //!< Base Pointer pair of registers
			MA, //!< Memory Address pair of registers
			PC_PLUS, //!< Increse Program Counter
			SP_PLUS, //!< Increse Stack Pointer
			SP_MINUS, //!< Decrese Stack Pointer
			RPC_PLUS //!< Reset the step counter and increse Program Counter
		};

		//! Line select defines for the secondary reading data bus
		enum class BusB
		{
			AB = 0, //!< Main registers
			D, //!< Hidden data register
			Low = 1, //!< Low register selector of pair
			High = 0, //!< High register selector of pair
			Default = 0
		};

		//! Line select defines for the addressing data bus
		enum class BusC
		{
			PC = 0, //!< Full 16 bit Program Counter register
			MA, //!< Full 16 bit Memory Address register
			SP, //!< Full 16 bit Stack Pointer register
			XY, //!< Full 16 bit auxiliary register
			Default = 0
		};

		Step(BusAR busAR, BusAW busAW, BusB busB = BusB::Default, BusC busC = BusC::Default, unsigned char aluS = 0, bool aluM = false, bool aluC = false);

		const UCode &getUCode() const;

	private:
		UCode uCode; //!< Parsed microcode for the current step
};

#endif
