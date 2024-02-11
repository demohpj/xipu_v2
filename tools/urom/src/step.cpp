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

#include "step.h"

/**
 * Step constructor
 *
 * @param busAR Which line of the main reading data bus is selected
 * @param busAW Which line of the main writing data bus is selected
 * @param busB Which line of the secondary reading data bus is selected
 * @param busC Which line of the addressing data bus is selected
 * @param aluS Seleted bits for ALU_S flags
 * @param aluM Seleted bit for ALU_M flag
 * @param aluC Seleted bit for ALU_C flag
 */
Step::Step(BusAR busAR, BusAW busAW, BusB busB, BusC busC, unsigned char aluS, bool aluM, bool aluC)
{
	QVector<unsigned char> code = { 0, 0 };

	code[0] |= (static_cast<unsigned char>(busAR) << CODE_0_BUS_AR_POSITION);
	code[0] |= (static_cast<unsigned char>(busAW) << CODE_0_BUS_AW_POSITION);
	code[0] |= (static_cast<unsigned char>(busB) << CODE_0_BUS_B_POSITION);

	code[1] |= (static_cast<unsigned char>(busC) << CODE_1_BUS_C_POSITION);
	code[1] |= ((aluS & CODE_FLAG_S_MASK) << CODE_1_FLAG_S_POSITION);
	code[1] |= ((aluM ? 1 : 0) << CODE_1_FLAG_M_POSITION);
	code[1] |= ((aluC ? 1 : 0) << CODE_1_FLAG_C_POSITION);

	this->uCode = UCode(code[0], code[1]);
}

/**
 * Get a parsed microcode for the current step
 *
 * @return Parsed microcode
 */
const UCode &Step::getUCode() const
{
	return(this->uCode);
}
