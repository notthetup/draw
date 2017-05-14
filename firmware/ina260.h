#ifndef INA260_H
#define INA260_H

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c-poll.h"

#define INA260_CONFIG_REG 0x00
#define INA260_CURRENT_REG 0x01
#define INA260_BUS_VOLTAGE_REG 0x02
#define INA260_POWER_REG 0x03
#define INA260_MANUFACTURER_REG 0xFE

int ina260Setup(I2C_TypeDef *, uint8_t, uint8_t);

uint32_t ina260getVoltageInuV();
uint32_t ina260getCurrentInuA();
uint32_t ina260getPowerInmW();

#ifdef __cplusplus
}
#endif

#endif // INA260_H
