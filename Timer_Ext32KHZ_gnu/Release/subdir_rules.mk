################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/Applications/TI/ccs1031/ccs/tools/compiler/msp430-gcc-9.3.1.11_macos/bin/msp430-elf-gcc-9.3.1" -c -mmcu=msp430g2452 -mhwmult=none -I"/Applications/TI/ccs1031/ccs/ccs_base/msp430/include_gcc" -I"/Users/mafm/Desktop/MSP430/Testing_MSP430G2/Timer_Ext32KHZ_gnu" -I"/Applications/TI/ccs1031/ccs/tools/compiler/msp430-gcc-9.3.1.11_macos/msp430-elf/include" -Os -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"   -fno-threadsafe-statics $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


