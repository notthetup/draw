/***************************************************************************//**
 * @file retargetdebug.c
 * @brief Provide stdio retargeting to UART or USB CDC for debugging purposes.
 * @author Silicon Labs
 * @version 1.11
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "retargetdebug.h"

#define RETARGET_IRQ_NAME  USART0_RX_IRQHandler
#define RETARGET_CLK       cmuClock_USART0
#define RETARGET_IRQn      USART0_RX_IRQn
#define RETARGET_UART      USART0
#define RETARGET_TX        USART_Tx
#define RETARGET_RX        USART_Rx
#define RETARGET_LOCATION  USART_ROUTE_LOCATION_LOC4
#define RETARGET_TXPORT    gpioPortB
#define RETARGET_TXPIN     7
#define RETARGET_RXPORT    gpioPortB
#define RETARGET_RXPIN     8

/**************************************************************************//**
 * @brief Intializes UART/LEUART
 *****************************************************************************/
void RETARGET_SerialInit(void)
{
  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
  //GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInput, 0);

  USART_TypeDef *usart = RETARGET_UART;
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

  /* Enable peripheral clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(RETARGET_CLK, true);

  /* Configure USART for basic async operation */
  init.enable = usartDisable;
  USART_InitAsync(usart, &init);

  /* Enable pins at correct location */
  usart->ROUTE = /*USART_ROUTE_RXPEN |*/ USART_ROUTE_TXPEN | RETARGET_LOCATION;

  /* Finally enable it */
  USART_Enable(usart, usartEnable);

#if !defined( __CROSSWORKS_ARM ) && defined( __GNUC__ )
  setvbuf( stdout, NULL, _IONBF, 0 ); /*Set unbuffered mode for stdout (newlib)*/
#endif
}


/**************************************************************************//**
 * @brief Receive a byte from USART/LEUART and put into global buffer
 * @return -1 on failure, or positive character integer on sucesss
 *****************************************************************************/
int RETARGET_ReadChar(void)
{
  return -1;
}

/**************************************************************************//**
 * @brief Transmit single byte to USART/LEUART
 * @param data Character to transmit
 *****************************************************************************/
int RETARGET_WriteChar(char c)
{
  RETARGET_TX(RETARGET_UART, c);
  return c;
}
