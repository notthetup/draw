#include <libopencm3/efm32/i2c.h>

#define INA260_SLAVE_ADDRESS 0x80

#define INA260_CONFIG_REG 0x00
#define INA260_CURRENT_REG 0x01
#define INA260_BUS_VOLTAGE_REG 0x02
#define INA260_POWER_REG 0x03
#define INA260_MANUFACTURER_REG 0xFE


int ina260_init(int i2c);

int ina260_getV(int i2c);
int ina260_getC(int i2c);
int ina260_getP(int i2c);
