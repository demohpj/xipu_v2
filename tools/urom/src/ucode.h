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

#ifndef UCODE_H
#define UCODE_H

#include <QVector>

//! This class contains a parsed microcode of the single step
class UCode
{
	public:
		UCode();
		UCode(unsigned char code0, unsigned char code1);

		unsigned char getCode0() const;
		unsigned char getCode1() const;

	private:
		QVector<unsigned char> code = QVector<unsigned char>(2); //!< Low and high bytes of the microcode
};

#endif
