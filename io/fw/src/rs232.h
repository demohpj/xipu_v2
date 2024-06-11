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

#ifndef _RS232_H
#define _RS232_H

#include "type.h"

void rs232Init(void);

void rs232Send(unsigned char byte);
unsigned char rs232Receive(void);

unsigned char rs232GetSendBufferFree(void);

bool rs232RxIsData(void);

#endif
