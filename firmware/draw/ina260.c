#include "ina260.h"
#include "usbcdc.h"

int ina260_init(void){
	int rv = 0;
	uint8_t txbuf;
	uint8_t rxbuf [2] = {0,0};

	i2c_init(I2C0, true);
	rv = i2c_bus_freq_set(I2C0, 21000000, 93000);  /** High frequency peripheral clock */
	usb_printf("Bus RV %d\r\n", rv);

	txbuf = INA260_MANUFACTURER_REG;

	rv = i2c_write(I2C0, INA260_SLAVE_ADDRESS, &txbuf, 1);
	usb_printf("Write RV %d\r\n", rv);

	rv = i2c_read(I2C0, INA260_SLAVE_ADDRESS, rxbuf, 2);
	usb_printf("Read RV %d\r\n", rv);
	usb_printf("Man ID [0] is 0x%X\r\n", rxbuf[0]);
	usb_printf("Man ID [1] is 0x%X\r\n", rxbuf[1]);

	return rv;
}
