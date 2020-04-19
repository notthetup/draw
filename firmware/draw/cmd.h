#pragma once

#include <libopencm3/efm32/gpio.h>
#include "board.h"

void cmd_parser(char *, int);
void execute_cmd(void);
