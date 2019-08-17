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

/* Default AHB (core clock) frequency of Tomu board */
#define AHB_FREQUENCY 14000000

int main(void)
{
  int i;

  /* Make sure the vector table is relocated correctly (after the Tomu bootloader) */
  SCB_VTOR = 0x4000;

  /* Disable the watchdog that the bootloader started. */
  WDOG_CTRL = 0;

  /*Init I2C peripheral clock*/
  cmu_periph_clock_enable(CMU_I2C0);

  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 0); // PC0 - I2C0-SDA
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 1); // PC1 - I2C0-SCL

  gpio_set(GPIOC,0);
  gpio_set(GPIOC,1);

  usb_cdc_init();

  while(1) {
    usb_printf("hello world\r\n");

      // gpio_toggle(LED_RED_PORT, LED_RED_PIN);
      for(i = 0; i != 5000000; ++i) __asm__("nop");
  }
}
