# Copyright Luke A.C.A. Rieff 2020 - Robot Dog Project
# /

#
# Global definitions.
#

# Microcontroller Definitions.
MCPU := cortex-m7

# Defines the toolchain.
TOOLCHAIN ?= arm-none-eabi

# Defines the tools from the toolchain.
GCC ?= $(TOOLCHAIN)-gcc
AS ?= $(TOOLCHAIN)-as
LINKER ?= $(TOOLCHAIN)-gcc
SIZE ?= $(TOOLCHAIN)-size
OBJCOPY ?= $(TOOLCHAIN)-objcopy

# Other tools
FLASH ?= st-flash

#
# Global Arguments.
#

GCC_ARGS += -Os
GCC_ARGS += -mthumb
GCC_ARGS += -mcpu=$(MCPU)
GCC_ARGS += -Wall
GCC_ARGS += -I./include
GCC_ARGS += -I../shared/include
GCC_ARGS += -I./vendor
GCC_ARGS += -DCONTROLLER_COMPILE

LINKER_ARGS += -T linker.ld
LINKER_ARGS += -mthumb
LINKER_ARGS += -mcpu=$(MCPU)
LINKER_ARGS += --specs=nosys.specs 
LINKER_ARGS += -Wall
LINKER_ARGS += -lgcc

FLASH_ARGS += write
FLASH_ARGS += main.bin
FLASH_ARGS += 0x8000000

#
# Gets the source files.
#

SOURCES := $(shell find source -name "*.c")
SOURCES += $(shell find ../shared/source -name "*.c")

OBJECTS := $(SOURCES:.c=.arm.o)

#
# Make Rules.
#

%.arm.o: %.c
	$(GCC)  $(GCC_ARGS) -c $< -o $@

%.arm.o: %.s
	$(GCC) $(GCC_ARGS) -c $< -o $@

all: $(OBJECTS) startup.arm.o
	$(LINKER) $(LINKER_ARGS) $(OBJECTS) startup.arm.o -o main.elf
bin:
	$(OBJCOPY) main.elf -O binary main.bin
size:
	$(SIZE) --format=gnu main.elf
flash:
	$(FLASH) $(FLASH_ARGS)
clean:
	rm -rf $(OBJECTS) startup.arm.o main.bin