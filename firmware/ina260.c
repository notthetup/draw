#include "ina260.h"

#include <stdint.h>

float ina260getVoltage(I2C_TypeDef * I2Cx , uint8_t slave_addr)
{
  int ret = 0;
  uint8_t value [2];
  uint8_t bus_voltage_reg [1] = {INA260_BUS_VOLTAGE_REG};

  ret = i2cWriteReg(I2Cx, slave_addr, bus_voltage_reg);
  if (ret != 0){
    // printf("I2C Write Failed..\n");
  }

  ret = i2cReadReg(I2Cx, slave_addr, value);
  if (ret != 0){
    // printf("I2C Read Failed..\n");
  }

  return ((value[0] << 8) + value[1])*1.25;
}
