# Copyright Luke A.C.A. Rieff 2020 - Robot Dog Project
# /

#
# Global definitions.
#


# Defines the tools from the toolchain.
GCC ?= g++
AS ?= as
SIZE ?= size
OBJCOPY ?= objcopy

# Other tools
FLASH ?= st-flash

#
# Global Arguments.
#

GCC_ARGS += -std=c++17
GCC_ARGS += -pthread
GCC_ARGS += -I./include
GCC_ARGS += -I./vendor
GCC_ARGS += -I./vendor/sdk/sdk/sdk/include
GCC_ARGS += -I./vendor/sdk/sdk/sdk/src
GCC_ARGS +=  -I/usr/include/panda3d -L/usr/lib/panda3d
GCC_ARGS += $(shell pkg-config --cflags eigen3)

#
# Gets the source files.
#

SOURCES := $(shell find source -name "*.cc")

OBJECTS := $(SOURCES:.cc=.o)

#
# Make Rules.
#

prepare:
	git clone https://github.com/slamtec/rplidar_sdk vendor/sdk | true
	cd ./vendor/sdk/sdk \
		&& make all \
		&& cd ../../../
all:
	$(GCC) $(GCC_ARGS) $(SOURCES) vendor/sdk/sdk/output/Linux/Release/librplidar_sdk.a -o main.elf
bin:
	$(OBJCOPY) main.elf -O binary main.bin
size:
	$(SIZE) --format=gnu main.elf
clean:
	rm -rf $(OBJECTS) main.bin