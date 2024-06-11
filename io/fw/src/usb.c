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

#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <stm32f0xx_hal_pcd.h>
#include <system_stm32f0xx.h>

#include "usbd/usbd_desc.h"
#include "usbd/usbd_msc.h"
#include "usbd/usbd_storage_if.h"

#include "usb.h"

extern USBD_StorageTypeDef USBD_MSC_Fops; //!< USB MSC function handlers
extern PCD_HandleTypeDef USBD_PCD_Handle; //!< USB PCD function handlers

static USBD_HandleTypeDef usbHandle; //!< USB handle

//! Init USB and set up USB MSC
void usbInit(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	USBD_Init(&usbHandle, &USBD_MSC_Desc, 0);
	
	USBD_RegisterClass(&usbHandle, &USBD_MSC);
	USBD_MSC_RegisterStorage(&usbHandle, &USBD_MSC_Fops);
	
	USBD_Start(&usbHandle);
}

//! Take care of the USB IRQ event
void USB_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&USBD_PCD_Handle);
}
