#
# toolchain file for Arduino projects
#

# non info...
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_SYSTEM_VERSION 1)

# only static libraries
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# compiler tools
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_OBJDUMP avr-objdump)
set(CMAKE_AR avr-gcc-ar)
set(CMAKE_OBJCOPY avr-objcopy)
set(CMAKE_STRIP avr-strip)
set(CMAKE_RANLIB avr-gcc-ranlib)
set(CMAKE_LINKER avr-ld)

# The programmer to use, read avrdude manual for list
set(PROG_TYPE arduino)

## AVR Chip Configuration
# 16Mhz, this should match the crystal on your board,
set(F_CPU 16000000UL)
# CPU, you can find the list here:
# https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
# mmcu MUST be passed to both the compiler and linker
set(MCU atmega328p)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUD 9600)

# assember, C compiler and C++ compiler flags
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp")
set(CMAKE_C_FLAGS "-std=gnu11")
set(CMAKE_CXX_FLAGS "-std=gnu++11 -fpermissive -fno-exceptions -fno-threadsafe-statics -Wno-error=narrowing")

# Pass defines to compiler
add_definitions(
    -DF_CPU=${F_CPU}
    -DBAUD=${BAUD}
    -DARDUINO=101	# forces usage of Arduino.h iso WPrograms.h
)

# generic options vor C and C++ compiler
add_compile_options(
    -mmcu=${MCU} # MCU
    -MMD
    -Os # optimize
    -g
    -ffunction-sections
    -fdata-sections
    -flto
    -fno-fat-lto-objects
    #-Wall # enable warnings
    #-Wno-main
    #-Wundef
    #-pedantic
    #-Werror
    #-Wfatal-errors
    #-Wl,--relax,--gc-sections
    #-gdwarf-2
    #-funsigned-char
    #-funsigned-bitfields
    #-fpack-struct
    #-fshort-enums
    #-fno-split-wide-types
    #-fno-tree-scev-cprop
)

# options for the linker
add_link_options(
    -mmcu=${MCU}
    -flto
    -fuse-linker-plugin
    -Wl,--gc-sections
)
