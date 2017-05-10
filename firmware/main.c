// Copyright 2016 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>
#include <stdbool.h>

#include "usbconfig.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_i2c.h"
#include "em_usb.h"

#include "callbacks.h"
#include "descriptors.h"

#define USART0_LOCATION 4
#define I2C0_LOCATION 4

#define I2C_TRANSFER_TIMEOUT 3000

// The uptime of this application in milliseconds, maintained by the SysTick
// timer.
volatile uint32_t uptime_millis;

// This functions is injected into the Interrupt Vector Table, and will be
// called whenever the SysTick timer fires (whose interval is configured inside
// main() further below).
void SysTick_Handler() {
  uptime_millis++;
}

void SpinDelay(uint32_t millis) {
  // Calculate the time at which we need to finish "sleeping".
  uint32_t sleep_until = uptime_millis + millis;

  // Spin until the requested time has passed.
  while (uptime_millis < sleep_until);
}

int itoa(int value, char *sp, int radix)
{
    char tmp[16] = {0};// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign)
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}



void setupUSART(void){
  /* USART is a HFPERCLK peripheral. Enable HFPERCLK domain and USART0.
 * We also need to enable the clock for GPIO to configure pins. */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_USART0, true);

  /* Initialize with default settings and then update fields according to application requirements. */
  USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
  USART_InitAsync(USART0, &initAsync);

  /* Enable I/O and set location */
  USART0->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC4;
  /* Set GPIO mode. */
  /* To avoid false start, configure TX pin as initial high */
  GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_TX_PORT(USART0_LOCATION), AF_USART0_TX_PIN(USART0_LOCATION), gpioModePushPull, 1);
  GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_RX_PORT(USART0_LOCATION), AF_USART0_RX_PIN(USART0_LOCATION), gpioModeInput, 0);

  USART_Enable(USART0, usartEnable);
}

void USART_Tx_String(USART_TypeDef *usart, char* str){
  while(*str){
    USART_Tx(usart, (uint8_t)*str++);
  }
}

void setupI2C(void)
{
  int i;

  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_I2C0, true);

  I2C_Reset(I2C0);

  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

  /* Configure SDA/SCL */

  GPIO_Port_TypeDef sdaPort = (GPIO_Port_TypeDef)AF_I2C0_SDA_PORT(I2C0_LOCATION);
  uint8_t sdaPin = AF_I2C0_SDA_PIN(I2C0_LOCATION);
  GPIO_Port_TypeDef sclPort = (GPIO_Port_TypeDef)AF_I2C0_SCL_PORT(I2C0_LOCATION);
  uint8_t sclPin = AF_I2C0_SCL_PIN(I2C0_LOCATION);

  GPIO_PinModeSet(sclPort,sclPin, gpioModeWiredAnd, 1);
  GPIO_PinModeSet(sdaPort,sdaPin, gpioModeWiredAnd, 1);


  /* In some situations, after a reset during an I2C transfer, the slave
     device may be left in an unknown state. Send 9 clock pulses to
     set slave in a defined state. */
  for (i = 0; i < 9; i++)
  {
    GPIO_PinModeSet(sclPort, sclPin, gpioModeWiredAnd, 0);
    GPIO_PinModeSet(sclPort, sclPin, gpioModeWiredAnd, 1);
  }

  /* Enable pins at location */
  I2C0->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | I2C_ROUTE_LOCATION_LOC4;

  I2C0->CTRL |= I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;

  /* Initializing the I2C */
  I2C_Init(I2C0, &i2cInit);

}

I2C_TransferReturn_TypeDef I2C_Send(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq)
{
  I2C_TransferReturn_TypeDef ret;
  uint32_t timeout = I2C_TRANSFER_TIMEOUT;
  /* Do a polled transfer */
  ret = I2C_TransferInit(i2c, seq);
  while (ret != i2cTransferDone && timeout--)
  {
    ret = I2C_Transfer(i2c);
  }
  return ret;
}

int i2cReadReg(uint8_t addr, uint8_t *value)
{
  I2C_TransferSeq_TypeDef seq;

  seq.addr        = addr;
  seq.flags       = I2C_FLAG_READ;
  seq.buf[0].len  = 2;
  seq.buf[0].data = value;

  return I2C_Send(I2C0, &seq);
}

int i2cWriteReg(uint8_t addr, uint8_t *value)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr        = addr;
  seq.flags       = I2C_FLAG_WRITE;
  seq.buf[0].len  = 1;
  seq.buf[0].data = value;

  ret = I2C_Send(I2C0, &seq);

  return ret;
}

float getVoltage(){
  int ret = 0;
  uint8_t value [2];
  uint8_t slave_addr = 0x80;
  uint8_t bus_voltage_reg [1] = {0x02};

  ret = i2cWriteReg(slave_addr, bus_voltage_reg);
  if (ret != 0){
    USART_Tx_String(USART0, "I2C Write Failed..\r\n");
  }

  ret = i2cReadReg(slave_addr, value);
  if (ret != 0){
    USART_Tx_String(USART0, "I2C Read Failed..\r\n");
  }

  return ((value[0] << 8) + value[1])*1.25;
}

int main() {

  uint8_t value [2];
  uint8_t slave_addr = 0x80;
  uint8_t man_id_reg [1] = {0xFE};
  char dest[4];
  int ret = 0;
  float voltage = 0;

  // Runs the Silicon Labs chip initialisation stuff, that also deals with
  // errata (implements workarounds, etc).
  CHIP_Init();

  // Switch on the clock for GPIO. Even though there's no immediately obvious
  // timing stuff going on beyond the SysTick below, it still needs to be
  // enabled for the GPIO to work.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Sets up and enable the `SysTick_Handler' interrupt to fire once every 1ms.
  // ref: http://community.silabs.com/t5/Official-Blog-of-Silicon-Labs/Chapter-5-MCU-Clocking-Part-2-The-SysTick-Interrupt/ba-p/145297
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
    // Something went wrong.
    while (1);
  }

  // Set up a TX pin with the GPIO controller and configure it to be open
  // drain:
  //  - PB7 == green
  GPIO_PinModeSet(gpioPortB, 11, gpioModeWiredOrPullDown, 0);

  setupUSART();
  USBD_Init(&initstruct);

  USART_Tx_String(USART0, "\r\nInit...\r\n");

  setupI2C();

  USART_Tx_String(USART0, "I2C Ready..\r\n");
  //
  ret = i2cWriteReg(slave_addr, man_id_reg);
  if (ret != 0){
    USART_Tx_String(USART0, "I2C Write Failed..\r\n");
  }

  ret = i2cReadReg(slave_addr, value);
  if (ret != 0){
    USART_Tx_String(USART0, "I2C Read Failed..\r\n");
  }

  if (value[0] == 0x54 && value[1] == 0x49 ){
    USART_Tx_String(USART0, "I2C Test Passed..\r\n");
  }else{
    USART_Tx_String(USART0, "I2C Test Failed..\r\n");
  }

  // Blink infinitely
  while (1) {

    GPIO_PinOutClear(gpioPortB, 11);
    SpinDelay(100);
    GPIO_PinOutSet(gpioPortB, 11);
    SpinDelay(100);
    voltage = getVoltage();

    USART_Tx_String(USART0, "Bus Voltage : ");
    itoa((int)voltage, dest, 10);
    USART_Tx_String(USART0, dest);
    USART_Tx(USART0, '\r');
    USART_Tx(USART0, '\n');
    SpinDelay(400);
  }
}
