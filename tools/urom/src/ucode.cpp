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

#include "ucode.h"

/**
 * Default constructor.
 * It creates an object filled by zeros
 */
UCode::UCode()
{
	this->code[0] = 0;
	this->code[1] = 0;
}

/**
 * Constructor using for create a parsed microcode of the single step based on an input data
 *
 * @param code0 Low parsed microcode part
 * @param code1 High parsed microcode part
 */
UCode::UCode(unsigned char code0, unsigned char code1)
{
	this->code[0] = code0;
	this->code[1] = code1;
}

/**
 * Get a value of the low microcode part
 *
 * @return Low microcode part
 */
unsigned char UCode::getCode0() const
{
	return(this->code[0]);
}

/**
 * Get a value of the high microcode part
 *
 * @return High microcode part
 */
unsigned char UCode::getCode1() const
{
	return(this->code[1]);
}
