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

#ifndef _LED_H
#define _LED_H

#include "type.h"

void ledInit(void);

void ledSetRun(bool enable);
void ledSetError(bool enable);

bool ledGetRun(void);
bool ledGetError(void);

void ledSetCapslock(bool enable);

#endif
