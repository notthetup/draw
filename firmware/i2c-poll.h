#ifndef I2C_POLL_H
#define I2C_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_gpio.h"
#include "em_cmu.h"
#include "em_i2c.h"

#define I2C_TRANSFER_TIMEOUT 3000

void i2cSetup(I2C_TypeDef *, int);
int i2cReadReg(I2C_TypeDef *, uint8_t , uint8_t *);
int i2cWriteReg(I2C_TypeDef *, uint8_t , uint8_t *);

I2C_TransferReturn_TypeDef I2C_Send(I2C_TypeDef *, I2C_TransferSeq_TypeDef *);


#ifdef __cplusplus
}
#endif

#endif // I2C_POLL_H
