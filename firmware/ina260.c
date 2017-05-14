#include "ina260.h"

#include <stdint.h>
#include <stdio.h>

uint8_t slave_addr = 0;
I2C_TypeDef * i2c_port;

int ina260Setup(I2C_TypeDef * I2Cx, uint8_t ina260_i2c_location, uint8_t ina260_slave_addr)
{
  int ret = 0;
  uint8_t value [2];
  uint8_t man_id_reg [1] = {INA260_MANUFACTURER_REG};

  i2cSetup(I2Cx, ina260_i2c_location);

  ret = i2cWriteReg(I2Cx,(uint8_t) ina260_slave_addr, man_id_reg);
  if (ret != 0){
    printf("I2C Write Failed..\n");
  }

  ret = i2cReadReg(I2Cx, (uint8_t) ina260_slave_addr, value);
  if (ret != 0){
    printf("I2C Read Failed..\n");
  }

  if (ret == 0 && value[0] == 0x54 && value[1] == 0x49){
    i2c_port = I2Cx;
    slave_addr = ina260_slave_addr;
    return 0;
  }

  return -1;
}

uint32_t ina260getVoltageInuV()
{
  int ret = 0;
  uint8_t value [2];
  uint8_t bus_voltage_reg [1] = {INA260_BUS_VOLTAGE_REG};

  ret = i2cWriteReg(i2c_port, slave_addr, bus_voltage_reg);
  if (ret != 0){
    printf("I2C Write Failed..\n");
  }

  ret = i2cReadReg(i2c_port, slave_addr, value);
  if (ret != 0){
    printf("I2C Read Failed..\n");
  }

  return (uint32_t)((value[0] << 8) + value[1])*1250;
}


uint32_t ina260getCurrentInuA()
{
  int ret = 0;
  uint8_t value [2];
  uint8_t current_reg [1] = {INA260_CURRENT_REG};

  ret = i2cWriteReg(i2c_port, slave_addr, current_reg);
  if (ret != 0){
    printf("I2C Write Failed..\n");
  }

  ret = i2cReadReg(i2c_port, slave_addr, value);
  if (ret != 0){
    printf("I2C Read Failed..\n");
  }

  return (uint32_t)((value[0] << 8) + value[1])*1250;
}


uint32_t ina260getPowerInmW()
{
  int ret = 0;
  uint8_t value [2];
  uint8_t power_reg [1] = {INA260_POWER_REG};

  ret = i2cWriteReg(i2c_port, slave_addr, power_reg);
  if (ret != 0){
    printf("I2C Write Failed..\n");
  }

  ret = i2cReadReg(i2c_port, slave_addr, value);
  if (ret != 0){
    printf("I2C Read Failed..\n");
  }

  return (uint32_t)((value[0] << 8) + value[1])*10;
}
