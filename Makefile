# set your MCU type here, or make command line `make MCU=MK20DX256`
MCU=MK20DX256

# The name of your project (used to name the compiled .hex file)
TARGET = $(notdir $(CURDIR))

# Base clock: 72000000
# Conservative overclock: 96000000
# Riskier overclocks: 120000000, 144000000
# Crazy overclock: 168000000
TEENSY_CORE_SPEED = 96000000

OPTIMIZE_LEVEL = -O

# configurable options
OPTIONS = -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE

# options needed by many Arduino libraries to configure for Teensy 3.0
OPTIONS += -D__$(MCU)__ -DF_CPU=$(TEENSY_CORE_SPEED) -DARDUINO=10600 -DTEENSYDUINO=121

# directory to build in
BUILDDIR = $(abspath $(CURDIR)/build)

# path location for Teensy 3 core
COREPATH = cores/teensy3

# path location for Arduino libraries
LIBRARYPATH = libraries

LIBRARIES = $(LIBRARYPATH)/Wire $(LIBRARYPATH)/Encoder $(LIBRARYPATH)/SPI $(LIBRARYPATH)/Servo $(LIBRARYPATH)/Adafruit_TCS34725

#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************
#
ifeq ($(OS),Windows_NT)
    # Windows machine
    $(error Using the Makefile on Windows is not supported. Simply open src/src.ino in Arduino with teensyduino and supporting libraries installed)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
    	# OS X Machine
    	ARDUINOPATH = /Applications/Arduino.app/Contents/Java
    else
    	# Other UNIX Machine
    	# This should probably be changed to where you extracted the Arduino app after downloading
    	ARDUINOPATH = ${HOME}/Downloads/arduino-1.6.6
    endif
endif

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(ARDUINOPATH)/hardware/tools

# path location for the arm-none-eabi compiler
COMPILERPATH = $(TOOLSPATH)/arm/bin

POSTCOMPILEPATH = $(TOOLSPATH)/teensy_post_compile
REBOOTPATH = $(TOOLSPATH)/teensy_reboot

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g $(OPTIMIZE_LEVEL) -ffunction-sections -fdata-sections -nostdlib -mcpu=cortex-m4 -mthumb -MMD $(OPTIONS) -I$(COREPATH)

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LOWER_MCU := $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(MCU)))))))))))))))))))))))))))
MCU_LD = $(LOWER_MCU).ld
LDSCRIPT = $(COREPATH)/$(MCU_LD)
LDFLAGS = $(OPTIMIZE_LEVEL) -Wl,--gc-sections,--defsym=__rtc_localtime=0 -mcpu=cortex-m4 -mthumb -T$(LDSCRIPT)

# additional libraries to link
LIBS = -lm

# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
LC_FILES := $(foreach library, $(LIBRARIES), $(wildcard $(library)/*.c))
LCPP_FILES := $(foreach library, $(LIBRARIES), $(wildcard $(library)/*.cpp))
TC_FILES := $(wildcard $(COREPATH)/*.c)
TCPP_FILES := $(wildcard $(COREPATH)/*.cpp)
TCPP_FILES := $(filter-out $(COREPATH)/main.cpp, $(TCPP_FILES))
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp)
INO_FILES := $(wildcard src/*.ino)

# include paths for libraries
L_INC := $(foreach lib,$(filter %/, $(wildcard $(LIBRARYPATH)/*/)), -I$(lib))

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(INO_FILES:.ino=.o) $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: hex

build: $(TARGET).elf

hex: $(TARGET).hex

post_compile: $(TARGET).hex
	@$(abspath $(POSTCOMPILEPATH)) -file="$(basename $<)" -path=$(CURDIR) -tools="$(abspath $(TOOLSPATH))"

reboot:
	@-$(abspath $(REBOOTPATH))

upload: post_compile reboot

$(BUILDDIR)/%.o: %.c
	@echo "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.ino
	@echo "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -x c++ -include Arduino.h -c "$<"

$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

%.hex: %.elf
	@echo "[HEX]\t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex"
