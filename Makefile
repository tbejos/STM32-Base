# Makefile for kernel demo

TOOLS   = arm-none-eabi
AS      = $(TOOLS)-as
CC      = $(TOOLS)-gcc
LD      = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP    = $(TOOLS)-objdump -D
GDB     = $(TOOLS)-gdb
MKDIR_P = mkdir -p
CP      = cp
SYMLINK = lns -af

############### LINKER SCRIPT ##########################

MCU_LINKER      = STM32F411CEUX_FLASH.ld
MCU_HEADER      = -DSTM32F411xE

############### LIBRARY INCLUSION ######################

USER_PROJ       = default
FLOAT           = soft
DEBUG           = 1
USER_ARG        = 0

USER_PROJ_BUILD  = user
K_PROJ_BUILD     = kernel
USER_PROJ_COMMON = user_common
PROJ             = kernel
BUILD            = build
ASM              = asm
BIN              = bin

# Terminal color and modifier attributes. Make sure to handle when no terminal
# is connected.
# Return to the normal terminal colors
n := $(shell tty -s && tput sgr0)
# Red color
r := $(shell tty -s && tput setaf 1)
# Green color
g := $(shell tty -s && tput setaf 2)
# Green color
y := $(shell tty -s && tput setaf 3)
j := $(shell tty -s && tput setaf 5)
# Bold text
b := $(shell tty -s && tput bold)
# Underlined text
u := $(shell tty -s && tput smul)

# BIN INFO
HASH_KERNEL = $(shell echo -n "$(DEBUG)$(OPTIMIZATION)$(FLOAT)" | md5sum | cut -d' ' -f1)
HASH_USER   = $(shell echo -n "$(DEBUG)$(OPTIMIZATION)$(FLOAT)$(USER_ARG)" | md5sum | cut -d' ' -f1)
BIN_DIR     = $(BUILD)/$(BIN)
BINARY      = $(PROJ)_$(USER_PROJ)_$(HASH_USER)

# KERNEL PROJ DIRS
K_ASM_DIR         = $(PROJ)/$(ASM)
K_INC_DIR         = $(PROJ)/include
K_LIB_DIR         = $(PROJ)/lib
K_SRC_DIR         = $(PROJ)/src
K_OBJ_DIR         = $(BUILD)/$(K_PROJ_BUILD)
K_OBJ_PROJ_DIR    = $(K_OBJ_DIR)/$(PROJ)_$(HASH_KERNEL)
PRE_BUILT_OBJ_DIR = $(BUILD)/prebuilt

# CMSIS DIRS
CMSIS            = CMSIS
ST_INC_DIR       = $(CMSIS)/Device/ST/STM32F4xx/Include
CMSIS_INC_DIR    = $(CMSIS)/Include

# SRC FILES
K_SRC        = $(wildcard $(K_SRC_DIR)/*.c)
K_ASM        = $(wildcard $(K_ASM_DIR)/*.S)

# RULES
K_OBJ_RULE        = $(K_SRC:$(K_SRC_DIR)/%.c=$(K_OBJ_PROJ_DIR)/%.o)
K_ASM_RULE        = $(K_ASM:$(K_ASM_DIR)/%.S=$(K_OBJ_PROJ_DIR)/%.o)

# OBJECTS
K_OBJECTS         = $(wildcard $(K_OBJ_PROJ_DIR)/*.o)
PRE_BUILT_OBJECTS = $(wildcard $(PRE_BUILT_OBJ_DIR)/*.o)

# Final output
OUTPUT = $(BIN_DIR)/$(BINARY)

# Path to soft float lib
ifeq ($(FLOAT), soft)
	FLOAT_ARCH += -mfloat-abi=soft
else
	FLOAT_ARCH += -mfloat-abi=hard -mfpu=fpv4-sp-d16  -march=armv7e-m
endif

# DEBUGGING is enabled by default, you can reduce binary size by disabling the
# DEBUGGING
ifeq ($(DEBUG), 1)
	DEFINE_MACROS = -DDEBUG -g
	OPTIMIZATION = -O0
else
	OPTIMIZATION = -O3 -funroll-all-loops
endif

ARCH                 = $(ARG) $(FLOAT_ARCH) -mslow-flash-data -mcpu=cortex-m4 -mlittle-endian -mthumb -ffreestanding
COMPILER_ERROR_FLAGS = -std=gnu99 -Wall -Werror -Wshadow -Wextra -Wunused
C_LIB_FLAG           = -nostdlib
DEFINE_MACROS         += $(MCU_HEADER)
CCFLAGS              += $(ARCH) $(COMPILER_ERROR_FLAGS) $(C_LIB_FLAG) $(OPTIMIZATION) $(DEFINE_MACROS)
K_CCFLAGS            = $(CCFLAGS) -nostartfiles
U_CCFLAGS            = $(CCFLAGS)

########################################################

################### ROOT RULES #########################
.PHONY: help setup flash doc clean veryclean $(BIN_DIR)/$(BINARY).elf
.SILENT:setup flash
# COMMENT LINE FOR VERBOSE LINKING
.SILENT:$(BIN_DIR)/$(BINARY).elf

help:
	@printf "$bSTM32F4 Makefile Usage:$n\n"
	@printf "\n"
	@printf "$bTargets:$n\n"
	@printf "\t$bbuild$n\n"
	@printf "\t    Compiles and links the specified $bPROJ$n and $bUSER_PROJ$n.\n"
	@printf "\t    If no $bPROJ$n,$bUSER_PROJ$n is specified then kernel will be linked with default.\n"
	@printf "\n"
	@printf "\t$bflash$n\n"
	@printf "\t    Compile, link and upload binary to board over serial.\n"
	@printf "\t    Be sure to run $bwindow_ocd.batch$n if you are in windows.\n"
	@printf "\t    Be sure to run $b./linux.ocd$n if you are in linux/mac.\n"
	@printf "\n"
	@printf "\t$bview-dump$n\n"
	@printf "\t    Compile, link and show disassembled binary.\n"
	@printf "\n"
	@printf "\t$bdoc$n\n"
	@printf "\t    Builds doxygen and ouputs into $bdoxygen_docs$n.\n"
	@printf "\t    Check $bdoxygen.warn$n for errors\n"
	@printf "\n"
	@printf "\t$bclean$n\n"
	@printf "\t    Cleans up all of the files generated by compilation in the\n"
	@printf "\t    $b$(BUILD)$n directory.\n"
	@printf "\n"
	@printf "\t$bcleandoc$n\n"
	@printf "\t    Cleans up generated doxygen files\n"
	@printf "\n"
	@printf "$bVariables:$n\n"
	@printf "\t$bPROJ$n\n"
	@printf "\t    The code to run in supervisor mode.\n"
	@printf "\n"
	@printf "\t$bOPTIMIZATION$n\n"
	@printf "\t    Sets the optimization level eg - $b-O3/-Os$n\n"
	@printf "\n"
	@printf "\t$bFLOAT$n\n"
	@printf "\t    Use soft or hard floating point libraries\n"
	@printf "\n"
	@printf "$bExamples:$n\n"
	@printf "\tmake build\n"
	@printf "\tmake flash\n"

compile: $(BIN_DIR)/$(BINARY).bin
	@printf "\n$y$bBuilt PROJ=$(PROJ) with USER_PROJ=$(USER_PROJ), FLOAT=$(FLOAT), DEBUG=$(DEBUG), OPTIMIZATION=$(OPTIMIZATION)\n$n$n"

setup:
	$(MKDIR_P) $(BUILD)
	$(MKDIR_P) $(BIN_DIR)
	$(MKDIR_P) $(K_OBJ_DIR)
	$(MKDIR_P) $(K_OBJ_PROJ_DIR)

build : setup compile

flash:	build
	cp util/init_template.nc /tmp/init.nc
	sed -i -e 's|<template>|$(BINARY)|g' /tmp/init.nc
	cp util/init_template.gdb /tmp/init.gdb
	sed -i -e 's|<template>|$(BINARY)|g' /tmp/init.gdb
	@printf "\n"
	@printf "$y***************************************************************\n$n"
	@printf "$yFlashing $(BINARY) to board...\n$n"
	@printf "$y***************************************************************\n$n"
	cat /tmp/init.nc | nc localhost 4444
	@printf "Opening GDB...\n"
	$(GDB) -x /tmp/init.gdb

view-dump: build dump

dump:
	$(DUMP) $(BIN_DIR)/$(BINARY).elf | less

########################################################

################# COMPILATION RULES ####################

$(K_OBJ_PROJ_DIR)/%.o: $(K_SRC_DIR)/%.c
	@printf "\n$b$yCompiling: $<$n$n\n" $<
	$(CC) -I$(K_INC_DIR) -I$(CMSIS_INC_DIR) -I$(ST_INC_DIR) $(K_CCFLAGS) -c $< -o $@

$(K_OBJ_PROJ_DIR)/%.o: $(K_ASM_DIR)/%.S
	@printf "\n$y$bAssembling: $<$n$n\n"
	$(AS) $< -o $@

########################################################

################### BINARY RULES #######################

$(BIN_DIR)/$(BINARY).bin:	$(BIN_DIR)/$(BINARY).elf
	@printf "\n$y$bCreating $(BINARY) binary file...$n$n\n"
	$(OBJCOPY) $(BIN_DIR)/$(BINARY).elf $(BIN_DIR)/$(BINARY).bin -O binary

$(BIN_DIR)/$(BINARY).elf: $(K_OBJ_RULE) $(K_ASM_RULE)
	cp util/$(MCU_LINKER) /tmp/linker.lds
	sed -i -e 's|<K_OBJ_DIR>|$(K_OBJ_PROJ_DIR)|g' /tmp/linker.lds
	sed -i -e 's|<U_OBJ_DIR>|$(U_OBJ_PROJ_DIR)|g' /tmp/linker.lds
	@printf "\n$y$bLinking $(BINARY)...$n$n\n"
	$(LD) -T /tmp/linker.lds -o $(BIN_DIR)/$(BINARY).elf $(K_OBJECTS) $(PRE_BUILT_OBJECTS)

########################################################

################### CLEANING RULES #####################

clean:
	$(RM) $(BIN_DIR)/*
	$(RM) -r $(K_OBJ_DIR)/*

veryclean: clean
	$(RM) doxygen.warn
	$(RM) -r doxygen_docs

########################################################

################### DOC RULES #########################

doc:
	doxygen util/doxygen/Doxyfile

cleandoc:
	$(RM) doxygen.warn
	$(RM) -r doxygen_docs
