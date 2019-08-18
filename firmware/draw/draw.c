/*
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include "usbcdc.h"
#include "ina260.h"

#include "toboot.h"
TOBOOT_CONFIGURATION(0);

void udelay_busy(uint32_t);
void board_init(void);

/* This busywait loop is roughly accurate when running at 24 MHz. */
void udelay_busy(uint32_t usecs){
    while (usecs --> 0) {
        /* This inner loop is 3 instructions, one of which is a branch.
         * This gives us 4 cycles total.
         * We want to sleep for 1 usec, and there are cycles per usec at 24 MHz.
         * Therefore, loop 6 times, as 6*4=24.
         */
        __asm("mov   r1, #6");
        __asm("retry:");
        __asm("sub r1, #1");
        __asm("bne retry");
        __asm("nop");
    }
}

void board_init(){
  /*Init peripheral clocks*/
  cmu_periph_clock_enable(CMU_GPIO);
  cmu_periph_clock_enable(CMU_I2C0);

  /*Setup Pins for I2C*/
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 0); // PC0 - I2C0-SDA
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 1); // PC1 - I2C0-SCL

  gpio_set(GPIOC,0);
  gpio_set(GPIOC,1);
}


int main(void)
{
  int rv;
  /* Disable the watchdog that the bootloader started. */
  WDOG_CTRL = 0;

  board_init();
  usb_cdc_init();

  usb_cdc_wait_for_start();

  rv = ina260_init(I2C0);
  if (rv < 0){
    usb_printf("# INA260 Init Error %d\r\n", rv);
  }

  while(1) {
    int v = ina260_getV(I2C0);
    udelay_busy(100);
    int c = ina260_getC(I2C0);
    udelay_busy(100);
    int p = ina260_getP(I2C0);

    usb_printf("V : %d, C : %d, P : %d \r\n", v, c, p);
    udelay_busy(300000);
  }
}
