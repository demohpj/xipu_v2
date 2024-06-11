/**
  ******************************************************************************
  * @file    usbd_msc_storage_template.c
  * @author  MCD Application Team
  * @brief   Memory management layer
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"
#include "../usb.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define STORAGE_LUN_NBR                  1U
#define STORAGE_BLK_NBR                  USB_FS_SECTOR_QUANTITY
#define STORAGE_BLK_SIZ                  USB_FS_SECTOR_SIZE

int8_t STORAGE_Init(uint8_t lun);

int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num,
                           uint16_t *block_size);

int8_t  STORAGE_IsReady(uint8_t lun);

int8_t  STORAGE_IsWriteProtected(uint8_t lun);

int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
                    uint16_t blk_len);

int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
                     uint16_t blk_len);

int8_t STORAGE_GetMaxLun(void);

/* USB Mass storage Standard Inquiry Data */
int8_t STORAGE_Inquirydata[] =  /* 36 */
{
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'X', 'i', 'P', 'C', 0x0, 0x0, 0x0, 0x0,
  'S', 't', 'o', 'r', 'a', 'g', 'e', ' ',
  'v', '2', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0
};

USBD_StorageTypeDef USBD_MSC_Fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  STORAGE_Inquirydata,

};

/**
  * @brief  Initializes the storage unit (medium)
  * @param  lun: Logical unit number
  * @retval Status (0 : OK / -1 : Error)
  */
int8_t STORAGE_Init(uint8_t lun)
{
  return(USBD_OK);
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number
  * @param  block_num: Number of total block number
  * @param  block_size: Block size
  * @retval Status (0: OK / -1: Error)
  */
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  UNUSED(lun);

  *block_num  = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;

  return(USBD_OK);
}


/**
  * @brief  Checks whether the medium is ready.
  * @param  lun: Logical unit number
  * @retval Status (0: OK / -1: Error)
  */
int8_t  STORAGE_IsReady(uint8_t lun)
{
  return(USBD_OK);
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number
  * @retval Status (0: write enabled / -1: otherwise)
  */
int8_t  STORAGE_IsWriteProtected(uint8_t lun)
{
  return(USBD_OK);
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number
  * @param  buf: data buffer
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0: OK / -1: Error)
  */
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf,
                    uint32_t blk_addr, uint16_t blk_len)
{
  for(uint32_t i = 0; i < blk_len; i++)
  {
    if(!flashRead((blk_addr + i), (struct FlashSector *)(buf + (FLASH_SECTOR_SIZE * i))))
    {
      return(USBD_FAIL);
    }
  }

  return(USBD_OK);
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number
  * @param  buf: data buffer
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0 : OK / -1 : Error)
  */
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf,
                     uint32_t blk_addr, uint16_t blk_len)
{
  for(uint32_t i = 0; i < blk_len; i++)
  {
    if(!flashWrite((blk_addr + i), (struct FlashSector *)(buf + (FLASH_SECTOR_SIZE * i))))
    {
      return(USBD_FAIL);
    }
  }

  return(USBD_OK);
}

/**
  * @brief  Returns the Max Supported LUNs.
  * @param  None
  * @retval Lun(s) number
  */
int8_t STORAGE_GetMaxLun(void)
{
  return(STORAGE_LUN_NBR - 1);
}


