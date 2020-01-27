#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/efm32/wdog.h>
#include <libopencm3/efm32/gpio.h>
#include <libopencm3/efm32/cmu.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

enum usbd_request_return_codes cdcacm_control_request(usbd_device *usbd_dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len, void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req));
void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep);
void cdcacm_set_config(usbd_device *usbd_dev, uint16_t wValue);
void usb_printf(char* format, ...);
void usb_cdc_init(void);
void usb_puts(char *s);

void usb_cdc_set_callback(void (*cb)(char *, int));
