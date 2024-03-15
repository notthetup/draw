# Draw

INA260 based voltage and current __draw__ measurement board.

This is a INA260 based board for measuring voltage and current draw (with builtin shunt) and output over USB (CDC).

## V2.0 - Under development.

The idea with v2 is to use an ESP32-C3. It's a cheap micro-controller. It has WiFi which may be handy, and it has USB.

While it's possible to use a ESP32-C3 module like the [ESP32-C3-WROOM-02U-N4](https://www.digikey.sg/en/products/detail/espressif-systems/ESP32-C3-WROOM-02U-N4/15222540), one would have to deal with powering the module from USB (5V), the reset circuitry, and the USB-C connector. It's much simpler to use the [Seeed Studio XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) which has all those components built in for a very low price.

### Features

- Current/Voltage/Power measurement
- Configurable over USB/WiF
    - Sensor gain
    - Sensor offset ??
    - Update rate
    - Averaging modes
- Optionally Tie ground to USB ground ??
- Optionally connect sensor VIN to INA260 VDD
- VIN, GND, VOUT on terminal block connector
- Configurable alerts values and output
- External power supply (5V)
- SPI pulled out for display (optional)
- pulled out IO pins to control/trigger/sense activity

### General price breakdown

- Seeed Studio XIAO ESP32C3 - ~[US$4.99](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html?queryID=015416e491c3db2e8304a974f8ab6ce7&objectID=5431&indexName=bazaar_retailer_products)
- INA260 - ~[US$6.38](https://www.digikey.com/en/products/detail/texas-instruments/INA260AIPW/6589219)
- Other components - ~US$10.00
**Total** - ~US$21.37


## V1.0

Documented [on this tag](https://github.com/notthetup/draw/tree/v1.0)