#include "ina260.h"
#include "usbcdc.h"
#include "board.h"

static void udelay_busy(uint32_t usecs){
  while (usecs --> 0) {
    /* This inner loop is 3 instructions, one of which is a branch.
     * This gives us 4 cycles total.
     * We want to sleep for 1 usec, and there are cycles per usec at 24 MHz.
     * Therefore, loop 6 times, as 6*4=24.
     */
    __asm("mov   r1, #6");
    __asm("retry:");
    __asm("sub r1, #1");
    __asm("bne retry");
    __asm("nop");
  }
}

int ina260_getReg(int i2c, uint8_t reg, uint8_t* rxbuf){
	int rv = i2c_write(i2c, INA260_SLAVE_ADDRESS, &reg, 1, false);
	if (rv < 0) return rv;
	return i2c_read(i2c, INA260_SLAVE_ADDRESS, rxbuf, 2, true);
}

int ina260_setReg(int i2c, uint8_t reg, uint8_t* val){
	uint8_t txbuf [3] = {reg,val[0],val[1]};
	return i2c_write(i2c, INA260_SLAVE_ADDRESS, txbuf, 3, true);
}

int ina260_init(int i2c){
	uint8_t rxbuf [2] = {0,0};

	i2c_init(i2c, true);
	i2c_bus_freq_set(i2c, 21000000, 400000);
	udelay_busy(2000);

	int rv = ina260_getReg(i2c, INA260_MANUFACTURER_REG, rxbuf);
	if (rv < 0) return rv;
	if(rxbuf[0] != 0x54 || rxbuf[1] != 0x49) return -9;
	return rv;
}

int int260_enableReadInterrupt(){
	gpio_mode_setup(ALERT_PORT, GPIO_MODE_INPUT, ALERT_PIN);
	// TODO Enable interrupt on this pin
}

int ina260_getC(int i2c, int* val){
	uint8_t rxbuf [2] = {0,0};
	int rv = ina260_getReg(i2c, INA260_CURRENT_REG, rxbuf);
	if (rv < 0) return rv;
	*val = ((int16_t)((rxbuf[0] << 8) + rxbuf[1]))*1250;
	return 0;
}

int ina260_getV(int i2c, int* val){
	uint8_t rxbuf [2] = {0,0};
	int rv = ina260_getReg(i2c, INA260_BUS_VOLTAGE_REG, rxbuf);
	if (rv < 0) return rv;
	*val = ((uint16_t)((rxbuf[0] << 8) + rxbuf[1]))*1250;
	return 0;
}


int ina260_getP(int i2c, int* val){
	uint8_t rxbuf [2] = {0,0};
	int rv = ina260_getReg(i2c, INA260_POWER_REG, rxbuf);
	if (rv < 0) return rv;
	*val = ((uint16_t)((rxbuf[0] << 8) + rxbuf[1]))*10;
	return 0;
}

int ina260_getConfig(int i2c, uint16_t* config){
	return ina260_getReg(i2c, INA260_CONFIG_REG, (uint8_t*)config);
}

int ina260_setConfig(int i2c, uint16_t config){
	return ina260_setReg(i2c, INA260_CONFIG_REG, (uint8_t*)(&config));
}

int ina260_getMaskEnable(int i2c, uint16_t* mask){
	return ina260_getReg(i2c, INA260_MASK_ENABLE_REG, (uint8_t*)mask);
}

int ina260_setMaskEnable(int i2c, uint16_t mask){
	return ina260_setReg(i2c, INA260_MASK_ENABLE_REG, (uint8_t*)(&mask));
}

int ina260_getAlertLimit(int i2c, uint16_t* limit){
	return ina260_getReg(i2c, INA260_ALERT_LIMIT_REG, (uint8_t*)limit);
}

int ina260_setAlertLimit(int i2c, uint16_t limit){
	return ina260_setReg(i2c, INA260_ALERT_LIMIT_REG, (uint8_t*)(&limit));
}
