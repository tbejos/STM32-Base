# Makefile for kernel demo

TOOLS   = arm-none-eabi
AS      = $(TOOLS)-as
CC      = $(TOOLS)-gcc
LD      = $(TOOLS)-ld
OBJCOPY = $(TOOLS)-objcopy
DUMP    = $(TOOLS)-objdump -D
MKDIR_P = mkdir -p
CP      = cp
GDB 	= gdb-multiarch
SYMLINK = lns -af

############### LINKER SCRIPT ##########################

MCU_LINKER      = STM32F411xE.ld

MCU_HEADER      = STM32F411xE

# SRC FILES
SRCS        = $(wildcard kernel/src/*.c)
SRCS        += $(wildcard kernel/asm/*.s)

PROJ_NAME=main

###################################################

CFLAGS  = -std=gnu99 -g -O2 -Wl,-Llinker,-Tlinker/$(MCU_LINKER) -D$(MCU_HEADER)
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -ffreestanding --specs=nosys.specs
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

CFLAGS += -Ikernel/include
CFLAGS += -ICMSIS/Include -ICMSIS/Device/ST/STM32F4xx/Include

###################################################

.PHONY: build

build: 	$(PROJ_NAME).elf

flash: 	build
		cp util/init_template.nc /tmp/init.nc
		sed -i -e 's|<template>|$(PROJ_NAME)|g' /tmp/init.nc
		cp util/init_template.gdb /tmp/init.gdb
		sed -i -e 's|<template>|$(PROJ_NAME)|g' /tmp/init.gdb
		@printf "\n"
		@printf "$y***************************************************************\n$n"
		@printf "$yFlashing $(PROJ_NAME) to board...\n$n"
		@printf "$y***************************************************************\n$n"
		cat /tmp/init.nc | nc localhost 4444
		@printf "Opening GDB...\n"
		$(GDB) -x /tmp/init.gdb

$(PROJ_NAME).elf: $(SRCS)
	$(MKDIR_P) bin
	$(CC) $(CFLAGS) $^ -o bin/$@ -lc
	$(OBJCOPY) -O ihex bin/$(PROJ_NAME).elf bin/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary bin/$(PROJ_NAME).elf bin/$(PROJ_NAME).bin

clean:
	rm -f bin/$(PROJ_NAME).elf
	rm -f bin/$(PROJ_NAME).hex
	rm -f bin/$(PROJ_NAME).bin
