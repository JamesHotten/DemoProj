################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.c \
../Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.c 

OBJS += \
./Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.o \
./Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.d \
./Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.d 


# 每个子目录必须为构建它所贡献的源提供规则
Drivers/CMSIS/DSP/Source/CommonTables/%.o Drivers/CMSIS/DSP/Source/CommonTables/%.su Drivers/CMSIS/DSP/Source/CommonTables/%.cyclo: ../Drivers/CMSIS/DSP/Source/CommonTables/%.c Drivers/CMSIS/DSP/Source/CommonTables/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-Source-2f-CommonTables

clean-Drivers-2f-CMSIS-2f-DSP-2f-Source-2f-CommonTables:
	-$(RM) ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.d ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.o ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.su ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.d ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.o ./Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.su ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.d ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.o ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.su ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.d ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.o ./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables_f16.su ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.d ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.o ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.su ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.d ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.o ./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs_f16.su ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.d ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.o ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables.su ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.cyclo ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.d ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.o ./Drivers/CMSIS/DSP/Source/CommonTables/arm_mve_tables_f16.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-Source-2f-CommonTables

