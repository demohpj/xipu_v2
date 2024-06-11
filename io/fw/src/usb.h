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

#ifndef _USB_H
#define _USB_H

#include "flash.h"

#define USB_FS_SECTOR_SIZE FLASH_SECTOR_SIZE //!< Sector size
#define USB_FS_SECTOR_QUANTITY FLASH_SECTOR_QUANTITY //!< Sector quantity

void usbInit(void);

#endif
