################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../si5351.syscfg 

C_SRCS += \
../si5351.c \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_2_04_00_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c \
../softiic_si5351.c 

GEN_CMDS += \
./device_linker.cmd 

GEN_FILES += \
./device_linker.cmd \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./si5351.d \
./ti_msp_dl_config.d \
./startup_mspm0g350x_ticlang.d \
./softiic_si5351.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./si5351.o \
./ti_msp_dl_config.o \
./startup_mspm0g350x_ticlang.o \
./softiic_si5351.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"si5351.o" \
"ti_msp_dl_config.o" \
"startup_mspm0g350x_ticlang.o" \
"softiic_si5351.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"si5351.d" \
"ti_msp_dl_config.d" \
"startup_mspm0g350x_ticlang.d" \
"softiic_si5351.d" 

GEN_FILES__QUOTED += \
"device_linker.cmd" \
"device.opt" \
"ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../si5351.c" \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_2_04_00_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c" \
"../softiic_si5351.c" 

SYSCFG_SRCS__QUOTED += \
"../si5351.syscfg" 


