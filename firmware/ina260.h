#ifndef INA260_H
#define INA260_H

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c-poll.h"

#define INA260_BUS_VOLTAGE_REG 0x02

float ina260getVoltage(I2C_TypeDef *, uint8_t);

#ifdef __cplusplus
}
#endif

#endif // INA260_H
