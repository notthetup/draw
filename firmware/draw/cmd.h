#pragma once

#include <stdio.h>
#include <libopencm3/efm32/gpio.h>
#include "board.h"
#include "usbcdc.h"

void cmd_parser(char *, int);
void execute_cmd(void);
