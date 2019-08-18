#include "ina260.h"
#include "usbcdc.h"

int ina260_init(int i2c){
	int rv = 0;
	uint8_t txbuf = INA260_MANUFACTURER_REG;
	uint8_t rxbuf [2] = {0,0};

	i2c_init(i2c, true);
	i2c_bus_freq_set(i2c, 21000000, 93000);

	rv = i2c_write(i2c, INA260_SLAVE_ADDRESS, &txbuf, 1, false);
	if (rv < 0) return rv;

	rv = i2c_read(i2c, INA260_SLAVE_ADDRESS, rxbuf, 2, true);
	if (rv < 0) return rv;

	if(rxbuf[0] != 0x54 || rxbuf[1] != 0x49) return -9;

	return rv;
}

int ina260_getV(int i2c){
	int rv = 0;
	uint8_t txbuf = INA260_BUS_VOLTAGE_REG;
	uint8_t rxbuf [2] = {0,0};

	rv = i2c_write(i2c, INA260_SLAVE_ADDRESS, &txbuf, 1, false);
	if (rv < 0) return rv;

	rv = i2c_read(i2c, INA260_SLAVE_ADDRESS, rxbuf, 2, true);
	if (rv < 0) return rv;

	return ((rxbuf[0] << 8) + rxbuf[1])*1250;
}

int ina260_getC(int i2c){
	int rv = 0;
	uint8_t txbuf = INA260_CURRENT_REG;
	uint8_t rxbuf [2] = {0,0};

	rv = i2c_write(i2c, INA260_SLAVE_ADDRESS, &txbuf, 1, false);
	if (rv < 0) return rv;

	rv = i2c_read(i2c, INA260_SLAVE_ADDRESS, rxbuf, 2, true);
	if (rv < 0) return rv;

	return ((rxbuf[0] << 8) + rxbuf[1])*1250;
}

int ina260_getP(int i2c){
	int rv = 0;
	uint8_t txbuf = INA260_POWER_REG;
	uint8_t rxbuf [2] = {0,0};

	rv = i2c_write(i2c, INA260_SLAVE_ADDRESS, &txbuf, 1, false);
	if (rv < 0) return rv;

	rv = i2c_read(i2c, INA260_SLAVE_ADDRESS, rxbuf, 2, true);
	if (rv < 0) return rv;

	return ((rxbuf[0] << 8) + rxbuf[1])*10;
}
