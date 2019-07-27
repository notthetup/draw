##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
## Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
## Copyright (C) 2012 Alexandru Gagniuc <mr.nuke.me@gmail.com>
## Copyright (C) 2018 Seb Holzapfel <schnommus@gmail.com>
##
## This library is free software: you can redistribute it and/or modify
## it undebipr the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

LIBNAME		= opencm3_efm32hg
DEFS		+= -DEFM32HG

FP_FLAGS	?= -mfloat-abi=soft
ARCH_FLAGS	= -mthumb -mcpu=cortex-m0plus $(FP_FLAGS)
DEFS            += -I../include

include ../libopencm3_rules.mk
