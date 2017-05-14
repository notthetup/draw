# Copyright 2016 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# Usee make V=1 for a verbose build.
ifndef V
        Q_CC		= @echo '      CC ' $@;
        Q_OBJDUMP	= @echo ' OBJDUMP ' $@;
        Q_OBJCOPY	= @echo ' OBJCOPY ' $@;
endif

################################################################################
#
# PATHS
#
# Put the path to the arm-none-eabi toolchain that you downloaded here, up to
# /bin/, and end with arm-none-eabi-
CROSS_COMPILE ?= $(patsubst %gcc,%,$(notdir $(shell which arm-none-eabi-gcc)))

# You can download the ARM toolchain from:
# https://launchpad.net/gcc-arm-embedded/+download

CC	= $(CROSS_COMPILE)gcc
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

################################################################################
#
# ARGUMENTS TO COMPILERS/LINKERS
#
IFLAGS = -I. \
				 -IGecko_SDK/platform/CMSIS/Include \
				 -IGecko_SDK/platform/Device/SiliconLabs/EFM32HG/Include \
				 -IGecko_SDK/platform/emlib/inc \
				 -IGecko_SDK/platform/middleware/usb_gecko/inc \
				 -IGecko_SDK/hardware/kit/common/drivers

CFLAGS = $(IFLAGS) \
				 -DEFM32HG309F64 \
				 -Wall \
				 -Wextra \
				 -mcpu=cortex-m0plus \
				 -mthumb \
				 -fmessage-length=0 \
				 -ffunction-sections \
				 -fdata-sections \
				 -fomit-frame-pointer \
				 -std=c99 \
				 -MMD \
				 -MP \
				 -Os \
				 -g \
				 -gdwarf-2 \
				 -DRETARGET_USART0

LSCRIPT = draw.ld

LFLAGS = -mcpu=cortex-m0plus \
 				 -gdwarf-2 \
				 -mthumb \
				 -T$(LSCRIPT) \
				 --specs=nano.specs \
				 -Wl,--gc-sections \
				 -Wl,--start-group \
				 -lgcc \
				 -lc \
				 -Wl,--end-group


################################################################################
#
# PHONY TARGETS
#
.PHONY: all draw check clean

all: draw

draw: draw.bin draw.elf
	arm-none-eabi-size draw.elf

check:
	@echo "Checking Gecko SDK linker script $(LSCRIPT) to ensure FLASH ORIGIN is set to 0x00004000..."
	@grep -E '^\s*FLASH.*ORIGIN\s+=\s+0x' $(LSCRIPT)
	@grep -q -E '^\s*FLASH.*ORIGIN\s+=\s+0x0*4000' $(LSCRIPT) || exit 1
	@echo "Checking Gecko SDK linker script $(LSCRIPT) to ensure FLASH LENGTH is set to 0xC000..."
	@grep -E '^\s*FLASH.*LENGTH\s+=\s+0x' $(LSCRIPT)
	@grep -q -E '^\s*FLASH.*LENGTH\s+=\s+0x0*C000' $(LSCRIPT) || exit 1
	@echo "Checking Gecko SDK linker script $(LSCRIPT) to ensure RAM ORIGIN is set to 0x20000000..."
	@grep -E '^\s*RAM.*ORIGIN\s+=\s+0x' $(LSCRIPT)
	@grep -q -E '^\s*RAM.*ORIGIN\s+=\s+0x20000000' $(LSCRIPT) || exit 1
	@echo "Checking Gecko SDK linker script $(LSCRIPT) to ensure RAM LENGTH is set to 0x2000..."
	@grep -E '^\s*RAM.*LENGTH\s+=\s+0x' $(LSCRIPT)
	@grep -q -E '^\s*RAM.*LENGTH\s+=\s+0x0*2000' $(LSCRIPT) || exit 1
	@echo "Checking ARM Toolchain is installed and accessible..."
	@echo " -> GCC"
	@test -x $(CC) || exit 1
	@echo " -> OBJCOPY"
	@test -x $(OBJCOPY) || exit 1
	@echo " -> OBJDUMP"
	@test -x $(OBJDUMP) || exit 1
	@echo "All checks pass."

clean:
	@echo "Cleaning .bin files..."
	@rm -f *.bin
	@echo "Cleaning .dump files..."
	@rm -f *.dump
	@echo "Cleaning .d files..."
	@rm -f *.d
	@echo "Cleaning .elf files..."
	@rm -f *.elf
	@echo "Cleaning .map files..."
	@rm -f *.map
	@echo "Cleaning .o files..."
	@rm -f *.o


################################################################################
#
# REAL TARGETS // COMMON
#
GECKO_A_SRC = Gecko_SDK/platform/Device/SiliconLabs/EFM32HG/Source/GCC/startup_efm32hg.S

GECKO_C_SRC = Gecko_SDK/platform/emlib/src/em_cmu.c \
							Gecko_SDK/platform/emlib/src/em_core.c \
							Gecko_SDK/platform/emlib/src/em_gpio.c \
							Gecko_SDK/platform/emlib/src/em_system.c \
							Gecko_SDK/platform/emlib/src/em_timer.c \
							Gecko_SDK/platform/emlib/src/em_usart.c \
							Gecko_SDK/platform/emlib/src/em_i2c.c \
							Gecko_SDK/hardware/kit/common/drivers/retargetio.c \
							Gecko_SDK/hardware/kit/common/drivers/retargetserial.c \
						  Gecko_SDK/platform/middleware/usb_gecko/src/em_usbd.c \
							Gecko_SDK/platform/middleware/usb_gecko/src/em_usbdch9.c \
							Gecko_SDK/platform/middleware/usb_gecko/src/em_usbdep.c \
							Gecko_SDK/platform/middleware/usb_gecko/src/em_usbdint.c \
							Gecko_SDK/platform/middleware/usb_gecko/src/em_usbhal.c \
							Gecko_SDK/platform/middleware/usb_gecko/src/em_usbtimer.c \
							Gecko_SDK/platform/Device/SiliconLabs/EFM32HG/Source/system_efm32hg.c

GECKO_OBJ = $(patsubst %.S,%.o,$(GECKO_A_SRC)) \
						$(patsubst %.c,%.o,$(GECKO_C_SRC))

BLINKY_A_SRC = $(shell find . -maxdepth 1 -name \*.S)
BLINKY_C_SRC = $(shell find . -maxdepth 1 -name \*.c)
BLINKY_OBJ = $(patsubst %.S,%.o,$(BLINKY_A_SRC)) $(patsubst %.c,%.o,$(BLINKY_C_SRC))

%.o: %.c
	$(Q_CC)$(CC) -c $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(Q_CC)$(CC) -c $(CFLAGS) -c -o $@ $<


################################################################################
#
# REAL TARGETS // CLEAN
#
draw.dump: draw.elf
	$(Q_OBJDUMP)$(OBJDUMP) -Sx $^ > $@

draw.elf: $(BLINKY_OBJ) $(GECKO_OBJ)
	$(Q_CC)$(CC) -Xlinker -Map=draw.map $(LFLAGS) -o $@ $^

draw.bin: draw.elf
	$(Q_OBJCOPY)$(OBJCOPY) -O binary $< $@


flash: draw.bin
	../upload.py --device /dev/tty.usbmodem1411 draw.bin
