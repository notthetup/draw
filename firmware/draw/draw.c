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
#include "utils.h"

#include "toboot.h"
TOBOOT_CONFIGURATION(0);

#define LED_GREEN_PORT GPIOA
#define LED_GREEN_PIN  GPIO0
#define LED_RED_PORT   GPIOB
#define LED_RED_PIN    GPIO7

char lastcmd = 0;
char lastlastcmd = 0;

void board_init(void);
void cmd_parser(char * buf, int len);
void execute_cmd(void);

void board_init() {
  /*Init peripheral clocks*/
  cmu_periph_clock_enable(CMU_GPIO);
  cmu_periph_clock_enable(CMU_I2C0);

  /*Setup Pins for I2C*/
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 0); // PC0 - I2C0-SDA
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 1); // PC1 - I2C0-SCL

  gpio_set(GPIOC,0);
  gpio_set(GPIOC,1);
}

void execute_cmd(){
  // usb_printf("# CMD : %c %c\r\n", lastlastcmd, lastcmd);
  if (lastlastcmd == 'L'){
    if (lastcmd == 'G') {
      gpio_toggle(LED_GREEN_PORT, LED_GREEN_PIN);
    }
    if (lastcmd == 'R') {
      gpio_toggle(LED_RED_PORT, LED_RED_PIN);
    }
  }
}

void cmd_parser(char * buf, int len){
  for (int i = 0; i < len; ++i) {
    // usb_printf("# %d:%d\r\n", i, buf[i]);
    if (buf[i] == '\r'){
      execute_cmd();
      lastlastcmd = 0;
      lastcmd = 0;
    }else{
      lastlastcmd = lastcmd;
      lastcmd = buf[i];
    }
  }
}


int main(void) {
  // int rv;
  /* Disable the watchdog that the bootloader started. */
  WDOG_CTRL = 0;

  board_init();
  usb_cdc_init();

  usb_cdc_set_callback(&cmd_parser);

  // rv = ina260_init(I2C0);
  // if (rv < 0){
  //   usb_printf("# INA260 Init Error %d\r\n", rv);
  // }

  // udelay_busy(2000); // delay 2ms to give time for first measurement to finish

  // uint16_t config = 0;
  // rv = ina260_getConfig(I2C0, &config);
  // if (rv == 0) usb_printf("Config : 0x%04X, \r\n", config);

  // rv = ina260_setConfig(I2C0, 0x2769);
  // if (rv < 0) usb_printf("ERR: O - %d\r\n", rv);

  // int pwr, vol, cur;
  // while(1) {
  //   rv = ina260_getP(I2C0, &pwr);
  //   if (rv < 0) usb_printf("ERR: P - %d\r\n", rv);
  //   rv = ina260_getC(I2C0, &cur);
  //   if (rv < 0) usb_printf("ERR: C - %d\r\n", rv);
  //   rv = ina260_getV(I2C0, &vol);
  //   if (rv < 0) usb_printf("ERR: V - %d\r\n", rv);
  //   usb_printf("V : %10d, C : %10d, P : %10d \r\n", vol, cur, pwr);
  //   udelay_busy(200000);
  // }

  while(1){}
}
