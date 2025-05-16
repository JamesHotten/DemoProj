################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs2010/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs2010/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/JamesHotten/Desktop/HCl/电赛/Demos/start53" --include_path="C:/ti/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/simplelink_msp432e4_sdk_4_20_00_12/source" --advice:power="all" --define=ccs --define=__MSP432E401Y__ -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs2010/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs2010/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/JamesHotten/Desktop/HCl/电赛/Demos/start53" --include_path="C:/ti/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/simplelink_msp432e4_sdk_4_20_00_12/source" --advice:power="all" --define=ccs --define=__MSP432E401Y__ -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


