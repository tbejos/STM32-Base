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

MCU_HEADER      = -DSTM32F411xE

# SRC FILES
K_SRC        = $(wildcard kernel/src/*.c)
K_ASM        = $(wildcard kernel/asm/*.s)

PROJ_NAME=main

###################################################

CFLAGS  = -std=gnu99 -g -O2 -Wall -Tutil/$(MCU_LINKER)
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

vpath %.a user_common/lib/hard_float

CFLAGS += -Ikernel/include
CFLAGS += -ICMSIS/Include -ICMSIS/Device/ST/STM32F4xx/Include

###################################################

.PHONY: proj

all: proj

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Luser_common/lib/hard_float
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	$(MAKE) -C lib clean
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
