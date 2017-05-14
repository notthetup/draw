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
#include <stdio.h>

#include "usbconfig.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_usb.h"

#include "i2c-poll.h"
#include "ina260.h"

#include "callbacks.h"
#include "descriptors.h"
#include "retargetserial.h"

#define I2C0_LOCATION 4

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

int main() {

  uint8_t value [2];
  uint8_t slave_addr = 0x80;
  uint8_t man_id_reg [1] = {0xFE};
  char str[20];
  int ret = 0;
  float voltage = 0;

  // Runs the Silicon Labs chip initialisation stuff, that also deals with
  // errata (implements workarounds, etc).
  CHIP_Init();

  // Switch on the clock for GPIO. Even though there's no immediately obvious
  // timing stuff going on beyond the SysTick below, it still needs to be
  // enabled for the GPIO to work.
  CMU_ClockEnable(cmuClock_GPIO, true);

  CMU_ClockEnable(cmuClock_HFPER, true);

  // Sets up and enable the `SysTick_Handler' interrupt to fire once every 1ms.
  // ref: http://community.silabs.com/t5/Official-Blog-of-Silicon-Labs/Chapter-5-MCU-Clocking-Part-2-The-SysTick-Interrupt/ba-p/145297
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
    // Something went wrong.
    while (1);
  }

  RETARGET_SerialInit();
  RETARGET_SerialCrLf(1);
  // USBD_Init(&initstruct);

  printf("\nInit...\n");

  // i2cSetup(I2C0, I2C0_LOCATION);
  //
  // printf("I2C Ready..\n");
  //
  // ret = i2cWriteReg(I2C0, slave_addr, man_id_reg);
  // if (ret != 0){
  //   printf("I2C Write Failed..\n");
  // }
  //
  // ret = i2cReadReg(I2C0, slave_addr, value);
  // if (ret != 0){
  //   printf("I2C Read Failed..\n");
  // }
  //
  // if (value[0] == 0x54 && value[1] == 0x49 ){
  //   printf("I2C Test Passed..\n");
  // }else{
  //   printf("I2C Test Failed..\n");
  // }

  // Blink infinitely
  while (1) {

    GPIO_PinOutClear(gpioPortB, 11);
    SpinDelay(100);
    GPIO_PinOutSet(gpioPortB, 11);
    SpinDelay(100);
    // voltage = ina260getVoltage(I2C0, slave_addr);
    //
    // sprintf(str, "Bus Voltage : %d\n", (int)voltage);
    // printf(str);
    SpinDelay(400);
  }
}
