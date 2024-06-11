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

#ifndef _FS_H
#define _FS_H

#include "type.h"

#define FS_NAME_MAX_LENGTH 12 //!< Maximum name length of the file
#define FS_PATH_MAX_LENGTH ((FS_NAME_MAX_LENGTH * 2) + 2) //!< Maximum path length to the file

#define FS_BLOCK_SIZE 256 //!<  Virtual block size available to address by communication bus between motherboard and CPU

#define FS_DIR_ENTRY_SIZE 16 //!< Size of the header with information about file
#define FS_DIR_SIZE_FIELD_SIZE 2 //!< Size of the size field in the header
#define FS_DIR_MAX_QUANTITY 64 //!< Maximum quantity of files in directory

#define FS_FILE_MAX_SIZE 53248 //!< Maximum size of the file

#define FS_MIN_CODE 0x20 //!< Minimum correctly character code
#define FS_MAX_CODE 0x7e //!< Maximum correctly character code

//! This struct contains a size of the file
struct FSSize
{
	unsigned char low; //!< Lower byte of the size
	unsigned char high; //!< Higher byte of the size
};

void fsInit(void);

bool fsOpen(const char *path, struct FSSize *size);
bool fsList(const char *path, struct FSSize *size);

unsigned char fsReadNextDataByte(void);

#endif
