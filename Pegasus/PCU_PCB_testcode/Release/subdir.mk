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
	"D:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -fno-exceptions -felide-constructors -std=gnu++0x -fno-rtti -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=130 -DARDUINO=10609 -DF_CPU=96000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH  -I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD" -I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility" -I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire" -I"D:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\utility" -I"D:\Users\Johnldean\Documents\Arduino\libraries\Adafruit_Sensor" -I"D:\Users\Johnldean\Documents\Arduino\libraries\Adafruit_BMP280" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '


