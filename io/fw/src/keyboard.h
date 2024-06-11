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

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "type.h"

#define KEYBOARD_FIRE_DELAY 50 //!< Delay between checking status of the keyboard input

void keyboardInit(void);
void keyboardFire(void);

unsigned char keyboardGetKey(void);

bool keyboardIsData(void);

#endif
