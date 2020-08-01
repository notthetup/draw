#include "board.h"

void board_init() {
  /*Init peripheral clocks*/

  /* Enable USHFRCO Oscillator */
  cmu_osc_on(USHFRCO);
  cmu_wait_for_osc_ready(USHFRCO);

  /* Use USHFRCO as main HFCLK */
  cmu_set_hfclk_source(USHFRCO);
  cmu_osc_off(CMU_OSCENCMD);

  /* Enable GPIO and I2C0 clocks */
  cmu_periph_clock_enable(CMU_GPIO);
  cmu_periph_clock_enable(CMU_I2C0);

  /*Setup Pins for I2C*/
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 0); // PC0 - I2C0-SDA
  gpio_mode_setup(GPIOC, GPIO_MODE_WIRED_AND, 1); // PC1 - I2C0-SCL

  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, 11); // PB11 - Alert

  gpio_set(GPIOC,0);
  gpio_set(GPIOC,1);
}
