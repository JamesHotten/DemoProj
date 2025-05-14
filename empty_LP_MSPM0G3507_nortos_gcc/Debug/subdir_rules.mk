################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/bin/arm-none-eabi-gcc-9.2.1.exe" -c @"device.opt"  -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -I"D:/css/empty_LP_MSPM0G3507_nortos_gcc" -I"D:/css/empty_LP_MSPM0G3507_nortos_gcc/Debug" -I"D:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"D:/TI/mspm0_sdk_2_04_00_06/source" -I"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/arm-none-eabi/include/newlib-nano" -I"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1534278321: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"E:/CHENHUI/ccs/utils/sysconfig_1.23.0/sysconfig_cli.bat" --script "D:/css/empty_LP_MSPM0G3507_nortos_gcc/empty.syscfg" -o "." -s "D:/TI/mspm0_sdk_2_04_00_06/.metadata/product.json" --compiler gcc
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.lds: build-1534278321 ../empty.syscfg
device.opt: build-1534278321
device.lds.genlibs: build-1534278321
ti_msp_dl_config.c: build-1534278321
ti_msp_dl_config.h: build-1534278321
Event.dot: build-1534278321

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/bin/arm-none-eabi-gcc-9.2.1.exe" -c @"device.opt"  -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -I"D:/css/empty_LP_MSPM0G3507_nortos_gcc" -I"D:/css/empty_LP_MSPM0G3507_nortos_gcc/Debug" -I"D:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"D:/TI/mspm0_sdk_2_04_00_06/source" -I"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/arm-none-eabi/include/newlib-nano" -I"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_gcc.o: D:/TI/mspm0_sdk_2_04_00_06/source/ti/devices/msp/m0p/startup_system_files/gcc/startup_mspm0g350x_gcc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/bin/arm-none-eabi-gcc-9.2.1.exe" -c @"device.opt"  -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -I"D:/css/empty_LP_MSPM0G3507_nortos_gcc" -I"D:/css/empty_LP_MSPM0G3507_nortos_gcc/Debug" -I"D:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"D:/TI/mspm0_sdk_2_04_00_06/source" -I"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/arm-none-eabi/include/newlib-nano" -I"E:/CHENHUI/gcc_arm_none_eabi_9_2_1/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


