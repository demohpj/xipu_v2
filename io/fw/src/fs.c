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

#include "fatfs/ff_gen_drv.h"

#include "type.h"
#include "flash.h"

#include "fs.h"

//! File system block type list
enum FSReadBlockType
{
	FS_READ_BLOCK_UNKNOWN = 0, //!< Unknown type
	FS_READ_BLOCK_FILE, //!< File type
	FS_READ_BLOCK_DIR, //!< Directory type
};

static DSTATUS fsFatInit(BYTE pdrv);
static DSTATUS fsFatGetStatus(BYTE pdrv);
static DRESULT fsFatRead(BYTE pdrv, BYTE *buf, DWORD sector, UINT count);

//! File system handlers to functions
static Diskio_drvTypeDef fatDriver =
{
	fsFatInit,
	fsFatGetStatus,
	fsFatRead
};

static char fatDiskPath[4]; //!< Path to the FAT file system
static FATFS fatFS; //!< Descriptor of the FAT file system

static FRESULT fatMountStatus = FR_NOT_READY; //!< Mount status of the physical drive
static FRESULT fatOpenStatus = FR_NOT_READY; //!< Open status of the file

static FIL fatFile; //!< Descriptor of the file

static unsigned int dataPos = 0; //!< Current read position in data
static unsigned int dataLength = 0; //!< Length of data

static int readBlockType = FS_READ_BLOCK_UNKNOWN; //!< Type of reading block
static unsigned char dirBuffer[FS_DIR_ENTRY_SIZE * FS_DIR_MAX_QUANTITY]; //!< Directory entrys buffer

//! Init file system and set up the FAT
void fsInit(void)
{
	if(!FATFS_LinkDriver(&fatDriver, fatDiskPath))
	{
		fatMountStatus = f_mount(&fatFS, fatDiskPath, 1);
	}
}

/**
 * Open a file
 *
 * @param path Path to the file
 * @param size Struct to store a size of the file
 *
 * @return Status of open operation
 */
bool fsOpen(const char *path, struct FSSize *size)
{
	size->low = 0;
	size->high = 0;
	
	readBlockType = FS_READ_BLOCK_FILE;
	
	if(!fatMountStatus)
	{
		if(!fatOpenStatus)
		{
			f_close(&fatFile);
		}
		
		fatOpenStatus = f_open(&fatFile, path, FA_READ);
		
		if(!fatOpenStatus)
		{
			DWORD fileSize = f_size(&fatFile);
			
			if(fileSize > FS_FILE_MAX_SIZE)
			{
				f_close(&fatFile);

				return(FALSE);
			}
			
			dataPos = 0;
			dataLength = fileSize;
			
			size->low = (fileSize % 256);
			size->high = ((fileSize / 256) & 0xff);
			
			return(TRUE);
		}
	}

	return(FALSE);
}

/**
 * List files in a directory
 *
 * @param path Path to the directory
 * @param size Struct to store a size of the list
 *
 * @return Status of list operation
 */
bool fsList(const char *path, struct FSSize *size)
{
	size->low = 0;
	size->high = 0;
	
	readBlockType = FS_READ_BLOCK_DIR;
	
	if(!fatMountStatus)
	{
		DIR dir;
		FILINFO info;
		
		if(!f_opendir(&dir, path))
		{
			int dirSize = 0;
			
			while(1)
			{
				if(f_readdir(&dir, &info) != FR_OK)
				{
					break;
				}
				
				if(info.fname[0] == 0)
				{
					break;
				}
				
				if(info.fsize > FS_FILE_MAX_SIZE)
				{
					continue;
				}
		
				for(int i = 0; i < FS_NAME_MAX_LENGTH; i++)
				{
					dirBuffer[dirSize + i] = info.fname[i];
				}
				
				dirBuffer[dirSize + FS_NAME_MAX_LENGTH] = 0;
				
				dirBuffer[dirSize + FS_DIR_ENTRY_SIZE - 2] = (info.fsize % 256);
				dirBuffer[dirSize + FS_DIR_ENTRY_SIZE - 1] = ((info.fsize / 256) & 0xff);
				
				dirSize += FS_DIR_ENTRY_SIZE;
				
				if(dirSize >= (FS_DIR_ENTRY_SIZE * FS_DIR_MAX_QUANTITY))
				{
					break;
				}
			}
			
			dataPos = 0;
			dataLength = dirSize;
			
			size->low = (dirSize % 256);
			size->high = ((dirSize / 256) & 0xff);
			
			return(TRUE);
		}
	}
	
	return(FALSE);
}

/**
 * Read next byte of the data
 *
 * @return Next byte of the data
 */
unsigned char fsReadNextDataByte(void)
{
	unsigned char data = 0;
	
	if((!fatMountStatus) && (!fatOpenStatus))
	{
		if(dataPos < dataLength)
		{
			if(readBlockType == FS_READ_BLOCK_FILE)
			{
				if(!f_lseek(&fatFile, dataPos))
				{
					UINT read;
					
					f_read(&fatFile, &data, 1, &read);
				}
			}
			else if(readBlockType == FS_READ_BLOCK_DIR)
			{
				data = dirBuffer[dataPos];
			}
			
			dataPos++;
		}
	}
	
	return(data);
}

/**
 * Get status of init
 *
 * @param pdrv Physical drive number
 *
 * @return Status of init
 */
static DSTATUS fsFatInit(BYTE pdrv)
{
	return(0);
}

/**
 * Get status of the physical drive
 *
 * @param pdrv Physical drive number
 *
 * @return Status of the physical drive
 */
static DSTATUS fsFatGetStatus(BYTE pdrv)
{
	return(0);
}

/**
 * Read sector of the physical drive
 *
 * @param pdrv Physical drive number
 * @param buf Buffer to write
 * @param sector Sector number to read
 * @param count Count of sectors to read
 *
 * @return Status of the reading process
 */
static DRESULT fsFatRead(BYTE pdrv, BYTE *buf, DWORD sector, UINT count)
{
	for(uint32_t i = 0; i < count; i++)
	{
		if(!flashRead((sector + i), (struct FlashSector *)(buf + (FLASH_SECTOR_SIZE * i))))
		{
			return(RES_ERROR);
		}
	}
	
	return(RES_OK);
}
