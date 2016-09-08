################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\PCU_PCB_testcode.cpp 

LINK_OBJ += \
.\PCU_PCB_testcode.cpp.o 

CPP_DEPS += \
.\PCU_PCB_testcode.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
PCU_PCB_testcode.cpp.o: ../PCU_PCB_testcode.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" "$@"
	@echo 'Finished building: $<'
	@echo ' '


