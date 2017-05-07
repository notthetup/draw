#! /usr/bin/env python

import logging
import serial
from xmodem import XMODEM, CRC
from time import sleep, time
import argparse

LOG = logging.getLogger(__name__)


def upload_firmware(dev, firmware_path):
    sdev = serial.Serial(port=dev, baudrate=115200, bytesize=8, parity='N',stopbits=1, timeout=None, xonxoff=0, rtscts=0)
    LOG.info("Uploading firmware to device %s", sdev.port)

    LOG.debug("Looking for the bootloader menu")
    sdev.write(b'i')
    line = sdev.readline()
    sleep(1.0)
    while sdev.inWaiting():
        line = sdev.readline()
        if b'BOOTLOADER' in line:
            LOG.info(line)
            break
        else:
            raise RuntimeError("Couldn't find bootloader menu")

    def getc(size, timeout=1):
        return sdev.read(size)

    def putc(data, timeout=1):
        sdev.write(data)

    LOG.debug("Sending firmware via XMODEM")
    with open(firmware_path, 'rb') as firmware:
        sdev.write(b'u')

        LOG.debug("Waiting for CRC character")
        start = time()
        while time() - start < 3.0:
            char = sdev.read(1)
            if char == CRC:
                break
        else:
            raise RuntimeError("Timeout waiting for CRC character")

        LOG.debug("Starting XMODEM transfer.")
        upload_success = XMODEM(getc, putc).send(firmware, retry=16, timeout=60*4, quiet=0)

        if upload_success:
            LOG.debug("Firmware uploaded to %s", sdev.port)
        else:
            raise RuntimeError("Error in XMODEM transfer")

        # Reset module into firmware
        LOG.info("New firmware uploaded and verified by module %s", sdev.port)
        sdev.write(b'r')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('firmware', help='path to the firmware bin to load')
    parser.add_argument('--device', help='Path of the device to program')
    args = parser.parse_args()

    if args.device and args.firmware:
        upload_firmware(args.device, args.firmware);


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    main()
