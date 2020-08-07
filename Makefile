# Makefile for kernel demo

TOOLS   = arm-none-eabi
AS      = $(TOOLS)-as
CC      = $(TOOLS)-gcc
LD      = $(TOOLS)-ld
OBJCOPY = $(TOOLS)-objcopy
DUMP    = $(TOOLS)-objdump -D
MKDIR_P = mkdir -p
CP      = cp
SYMLINK = lns -af

############### LINKER SCRIPT ##########################

MCU_LINKER      = STM32F411xE.ld

MCU_HEADER      = STM32F411xE

# SRC FILES
SRCS        = $(wildcard kernel/src/*.c)
SRCS        += $(wildcard kernel/asm/*.s)

PROJ_NAME=main

###################################################

CFLAGS  = -std=gnu99 -g -O2 -Wall -Llinker -Tlinker/$(MCU_LINKER) -D$(MCU_HEADER)
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork --specs=nosys.specs
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

CFLAGS += -Ikernel/include
CFLAGS += -ICMSIS/Include -ICMSIS/Device/ST/STM32F4xx/Include

###################################################

.PHONY: build

build: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(MKDIR_P) bin
	$(CC) $(CFLAGS) $^ -o bin/$@ -lc
	$(OBJCOPY) -O ihex bin/$(PROJ_NAME).elf bin/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary bin/$(PROJ_NAME).elf bin/$(PROJ_NAME).bin

clean:
	rm -f bin/$(PROJ_NAME).elf
	rm -f bin/$(PROJ_NAME).hex
	rm -f bin/$(PROJ_NAME).bin
