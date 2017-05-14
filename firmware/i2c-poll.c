
#include "i2c-poll.h"

void i2cSetup(I2C_TypeDef * I2Cx, int I2C_LOCATION)
{
  int i;

  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_I2C0, true); // TODO generic i2c

  I2C_Reset(I2Cx);

  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

  /* Configure SDA/SCL */

  GPIO_Port_TypeDef sdaPort = (GPIO_Port_TypeDef)AF_I2C0_SDA_PORT(I2C_LOCATION); // TODO generic i2c
  uint8_t sdaPin = AF_I2C0_SDA_PIN(I2C_LOCATION); // TODO generic i2c
  GPIO_Port_TypeDef sclPort = (GPIO_Port_TypeDef)AF_I2C0_SCL_PORT(I2C_LOCATION); // TODO generic i2c
  uint8_t sclPin = AF_I2C0_SCL_PIN(I2C_LOCATION); // TODO generic i2c

  GPIO_PinModeSet(sclPort,sclPin, gpioModeWiredAnd, 1);
  GPIO_PinModeSet(sdaPort,sdaPin, gpioModeWiredAnd, 1);


  /* In some situations, after a reset during an I2C transfer, the slave
     device may be left in an unknown state. Send 9 clock pulses to
     set slave in a defined state. */
  for (i = 0; i < 9; i++)
  {
    GPIO_PinModeSet(sclPort, sclPin, gpioModeWiredAnd, 0);
    GPIO_PinModeSet(sclPort, sclPin, gpioModeWiredAnd, 1);
  }

  /* Enable pins at location */
  I2Cx->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | I2C_ROUTE_LOCATION_LOC4;

  I2Cx->CTRL |= I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;

  /* Initializing the I2C */
  I2C_Init(I2Cx, &i2cInit);

}

int i2cReadReg(I2C_TypeDef * I2Cx, uint8_t addr, uint8_t *value)
{
  I2C_TransferSeq_TypeDef seq;

  seq.addr        = addr;
  seq.flags       = I2C_FLAG_READ;
  seq.buf[0].len  = 2;
  seq.buf[0].data = value;

  return I2C_Send(I2Cx, &seq);
}

int i2cWriteReg(I2C_TypeDef * I2Cx, uint8_t addr, uint8_t *value)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr        = addr;
  seq.flags       = I2C_FLAG_WRITE;
  seq.buf[0].len  = 1;
  seq.buf[0].data = value;

  ret = I2C_Send(I2Cx, &seq);

  return ret;
}


/************************* Helper Methods ***********************/

I2C_TransferReturn_TypeDef I2C_Send(I2C_TypeDef *I2Cx, I2C_TransferSeq_TypeDef *seq)
{
  I2C_TransferReturn_TypeDef ret;
  uint32_t timeout = I2C_TRANSFER_TIMEOUT;
  /* Do a polled transfer */
  ret = I2C_TransferInit(I2Cx, seq);
  while (ret != i2cTransferDone && timeout--)
  {
    ret = I2C_Transfer(I2Cx);
  }
  return ret;
}
