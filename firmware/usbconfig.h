/***************************************************************************//**
 * @file usbconfig.h
 * @brief USB protocol stack library, application supplied configuration options.
 * @author Silicon Labs
 * @version 1.10
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef __USBCONFIG_H
#define __USBCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define EP_DATA_OUT  0x01       /* Endpoint for USB data reception.       */
#define EP_DATA_IN   0x81       /* Endpoint for USB data transmission.    */
#define EP_NOTIFY    0x82       /* The notification endpoint (not used).  */

#define USB_DEVICE

/****************************************************************************
**                                                                         **
** Specify number of endpoints used (in addition to EP0).                  **
**                                                                         **
*****************************************************************************/
#define NUM_EP_USED 3

/****************************************************************************
**                                                                         **
** Specify number of application timers you need.                          **
**                                                                         **
*****************************************************************************/
#define NUM_APP_TIMERS 0

/****************************************************************************
**                                                                         **
** Configure serial port debug output.                                     **
**                                                                         **
*****************************************************************************/

#if defined( BL_DEBUG )
  /* Define a function for transmitting a single char on the serial port. */
  extern int RETARGET_WriteChar(char c);
  #define USER_PUTCHAR  RETARGET_WriteChar

  /* Debug USB API functions (illegal input parameters etc.) */
  #define DEBUG_USB_API

  /* Include the printf function. */
  #define USB_USE_PRINTF
#endif

#ifdef __cplusplus
}
#endif

#endif /* __USBCONFIG_H */
