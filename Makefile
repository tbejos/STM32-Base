# Makefile for kernel demo

TOOLS   = arm-none-eabi
AS      = $(TOOLS)-as
CC      = $(TOOLS)-gcc
LD      = $(TOOLS)-ld
OBJCOPY = $(TOOLS)-objcopy
DUMP    = $(TOOLS)-objdump -D
MKDIR_P = mkdir -p
CP      = cp
GDB 	= $(TOOLS)-gdb
SYMLINK = lns -af

############### LINKER SCRIPT #####################

MCU_HEADER      = STM32F411xE

MCU_LINKER      = $(MCU_HEADER).ld

# SRC FILES
SRCS        = $(wildcard kernel/src/*.c)
SRCS        += $(wildcard kernel/asm/*.s)

PROJ_NAME=main

###################################################

CFLAGS  = -std=gnu99 -g3 -O2 -D$(MCU_HEADER) -Wl,-Llinker,-Tlinker/$(MCU_LINKER)
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -ffreestanding --specs=nosys.specs --specs=nano.specs
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

CFLAGS += -Ikernel/include
CFLAGS += -ICMSIS/Include -ICMSIS/Device/ST/STM32F4xx/Include

###################################################

.PHONY: build flash clean

build: 	$(PROJ_NAME).elf

flash: 	build
		cp util/init_template.nc /tmp/init.nc
		sed -i -e 's|<template>|$(PROJ_NAME)|g' /tmp/init.nc
		cp util/init_template.gdb /tmp/init.gdb
		sed -i -e 's|<template>|$(PROJ_NAME)|g' /tmp/init.gdb
		@printf "\n"
		@printf "***************************************************************\n"
		@printf "Flashing $(PROJ_NAME) to board...\n"
		@printf "***************************************************************\n"
		cat /tmp/init.nc | nc localhost 4444
		@printf "Opening GDB...\n"
		$(GDB) -x /tmp/init.gdb

$(PROJ_NAME).elf: $(SRCS)
	$(MKDIR_P) bin
	$(CC) $(CFLAGS) $^ -o bin/$@
	$(OBJCOPY) -O ihex bin/$(PROJ_NAME).elf bin/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary bin/$(PROJ_NAME).elf bin/$(PROJ_NAME).bin

clean:
	rm -f bin/$(PROJ_NAME).elf
	rm -f bin/$(PROJ_NAME).hex
	rm -f bin/$(PROJ_NAME).bin
