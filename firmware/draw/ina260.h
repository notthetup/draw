#include <libopencm3/efm32/i2c.h>

#define INA260_SLAVE_ADDRESS 0x80

#define INA260_CONFIG_REG 			0x00
#define INA260_CURRENT_REG 			0x01
#define INA260_BUS_VOLTAGE_REG 	0x02
#define INA260_POWER_REG 				0x03
#define INA260_MASK_ENABLE_REG 	0x06
#define INA260_ALERT_LIMIT_REG 	0x07
#define INA260_MANUFACTURER_REG 0xFE

int ina260_getReg(int i2c, uint8_t reg, uint8_t* rxbuf);
int ina260_setReg(int i2c, uint8_t reg, uint8_t* txbuf);

int ina260_init(int i2c);

int ina260_getC(int i2c, int* val);
int ina260_getV(int i2c, int* val);
int ina260_getP(int i2c, int* val);

int ina260_getConfig(int i2c, uint16_t* config);
int ina260_setConfig(int i2c, uint16_t config);

int ina260_getMaskEnable(int i2c, uint16_t* mask);
int ina260_setMaskEnable(int i2c, uint16_t mask);

int ina260_getAlertLimit(int i2c, uint16_t* limit);
int ina260_setAlertLimit(int i2c, uint16_t limit);
