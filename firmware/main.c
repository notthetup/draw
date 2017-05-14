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

#define INA260_I2C0_LOCATION 4
#define INA260_SLAVE_ADDRESS 0x80

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
  bool ret;
  char str[30];
  uint32_t voltage, current, power= 0;

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

  ret = ina260Setup(I2C0, INA260_I2C0_LOCATION, INA260_SLAVE_ADDRESS);
  if (ret){
    printf("INA260 Ready..\n");
  }


  // Blink infinitely
  while (1) {

    GPIO_PinOutClear(gpioPortB, 11);
    SpinDelay(100);
    GPIO_PinOutSet(gpioPortB, 11);
    SpinDelay(100);
    voltage = ina260getVoltageInuV();
    current = ina260getCurrentInuA();
    power = ina260getPowerInmW();

    sprintf(str, "{V : %lu, C : %lu, P: %lu} \n", voltage, current, power);
    printf(str);
    SpinDelay(400);
  }
}
