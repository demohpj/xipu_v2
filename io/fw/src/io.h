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

#ifndef _IO_H
#define _IO_H

#define IO_FIRE_CLOCK_DELAY 125 //!< Delay between tick of the board clock. It is a 8 Hz clock used for delays and waits.
#define IO_FIRE_PERIPHERIAL_DELAY 10 //!< Delay used to check if a new data is waiting in keyboard or rs232 buffers

#define IO_IN_DATA_MASK 0x0f //!< Input data mask for the data part of the BUS
#define IO_IN_CONTROL_MASK 0xfff0 //!< Input data mask for the control bits part of the BUS

#define IO_IN_CLOCK_BIT (1 << 4) //!< Input clock control bit
#define IO_IN_KEYBOARD_READY_BIT (1 << 5) //!< Input "keyboard has data to read" control bit
#define IO_IN_RS232_READY_BIT (1 << 6) //!< Input "RS232 has data to read" control bit
#define IO_IN_OPERATION_COMPLETE_BIT (1 << 7) //!< Input operation completed control bit

#define IO_OUT_DATA_MASK 0x0f //!< Output data mask for the data part of the BUS
#define IO_OUT_CONTROL_MASK 0xf0 //!< Output data mask for the control bits part of the BUS

#define IO_OUT_HALF_BIT (1 << 4) //!< Output "half selector of the byte" control bit
#define IO_OUT_MODE_BIT (1 << 5) //!< Output "register selection or data transision mode" control bit
#define IO_OUT_RW_BIT (1 << 6) //!< Output read/write control bit
#define IO_OUT_DATA_READY_BIT (1 << 7) //!< Input "data is ready to read from output" control bit

#define IO_REG_SELECT_MASK 0x0f //!< Register selection mask

#define IO_HALF_UP_DATA_MASK 0xf0 //!< Upper half of the byte mask
#define IO_HALF_DATA_OFFSET 4 //!< Upper half of the byte offset

void ioPreInit(void);
void ioInit(void);
void ioFireClock(void);
void ioFirePeripherial(void);

#endif
