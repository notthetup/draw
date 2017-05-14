/**************************************************************************//**
 * @file bootldio.c
 * @brief IO code, USART or USB, for the EFM32 bootloader
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

#include <stdbool.h>
#include "em_device.h"
#include "em_usb.h"
#include "bootldio.h"

#define USB_BUF_SIZ 256

STATIC_UBUF( usbBuffer, USB_BUF_SIZ );

static uint32_t           usbXferCnt;
static volatile bool      usbXferDone;
static USB_Status_TypeDef usbXferStatus;

/**************************************************************************//**
 * @brief
 *    Callback function called whenever a packet with data has been
 *    transferred on USB.
 *****************************************************************************/
static int UsbDataXferred( USB_Status_TypeDef status,
                           uint32_t xferred, uint32_t remaining )
{
  (void)remaining;            /* Unused parameter */

  usbXferStatus = status;
  usbXferCnt    = xferred;
  usbXferDone   = true;

  return USB_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Prints an int in hex.
 *
 * @param integer
 *   The integer to be printed.
 ******************************************************************************/
void BOOTLDIO_printHex( uint32_t integer )
{
  uint8_t c;
  int     i, j, digit;

  for ( i = 7, j = 0; i >= 0; i--, j++ )
  {
    digit = (integer >> (i * 4)) & 0xf;
    if (digit < 10)
    {
      c = digit + 0x30;
    }
    else
    {
      c = digit + 0x37;
    }

    usbBuffer[ j ] = c;
  }

  usbBuffer[ j ] = '\0';
  BOOTLDIO_printString( usbBuffer );
}

/**************************************************************************//**
 * @brief Get single byte from USART or USB
 *****************************************************************************/
uint8_t BOOTLDIO_rxByte( void )
{
  uint8_t  retVal;

  usbXferDone = false;
  USBD_Read( EP_DATA_OUT, usbBuffer, USB_BUF_SIZ, UsbDataXferred );
  while ( !usbXferDone ){}
  retVal = usbXferStatus == USB_STATUS_OK ? usbBuffer[0] : 0;

  return retVal;
}

/**************************************************************************//**
 * @brief Transmit single byte to USART or USB
 *****************************************************************************/
int BOOTLDIO_txByte( uint8_t data )
{
  usbBuffer[ 0 ] = data;
  usbXferDone = false;
  USBD_Write( EP_DATA_IN, usbBuffer, 1, UsbDataXferred );
  while ( !usbXferDone ){}
  return usbXferStatus == USB_STATUS_OK ? (int)data : 0;

  return (int)data;
}

/**************************************************************************//**
 * @brief Transmit null-terminated string to USART or USB
 *****************************************************************************/
void BOOTLDIO_printString( const uint8_t *string )
{
  int len;
  len = strlen( (char*)string );
  memcpy( usbBuffer, string, len );
  usbXferDone = false;
  USBD_Write( EP_DATA_IN, usbBuffer, len, UsbDataXferred );
  while ( !usbXferDone ){}
}
