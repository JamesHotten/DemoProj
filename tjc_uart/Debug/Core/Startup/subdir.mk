################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
S_SRCS += \
../Core/Startup/startup_stm32f103c8tx.s 

OBJS += \
./Core/Startup/startup_stm32f103c8tx.o 

S_DEPS += \
./Core/Startup/startup_stm32f103c8tx.d 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f103c8tx.d ./Core/Startup/startup_stm32f103c8tx.o

.PHONY: clean-Core-2f-Startup

