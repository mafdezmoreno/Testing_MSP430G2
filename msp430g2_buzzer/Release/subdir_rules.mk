################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"" -c -mmcu=msp430g2452 -mhwmult=none -I"/Applications/ti/ccs1240/ccs/ccs_base/msp430/include_gcc" -I"/Users/mafm/Code/msp430/MSP430/Testing_MSP430G2/msp430g2_buzzer" -Os -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"   -fno-threadsafe-statics $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


