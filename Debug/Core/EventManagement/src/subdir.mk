################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/EventManagement/src/EventManagement.c \
../Core/EventManagement/src/EventManagement_Cfg.c 

OBJS += \
./Core/EventManagement/src/EventManagement.o \
./Core/EventManagement/src/EventManagement_Cfg.o 

C_DEPS += \
./Core/EventManagement/src/EventManagement.d \
./Core/EventManagement/src/EventManagement_Cfg.d 


# Each subdirectory must supply rules for building sources it contributes
Core/EventManagement/src/%.o Core/EventManagement/src/%.su Core/EventManagement/src/%.cyclo: ../Core/EventManagement/src/%.c Core/EventManagement/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/EventManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/ModeManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/HAL_Drivers/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Middleware/Schedular/CoperativeSchedular/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/BSP/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-EventManagement-2f-src

clean-Core-2f-EventManagement-2f-src:
	-$(RM) ./Core/EventManagement/src/EventManagement.cyclo ./Core/EventManagement/src/EventManagement.d ./Core/EventManagement/src/EventManagement.o ./Core/EventManagement/src/EventManagement.su ./Core/EventManagement/src/EventManagement_Cfg.cyclo ./Core/EventManagement/src/EventManagement_Cfg.d ./Core/EventManagement/src/EventManagement_Cfg.o ./Core/EventManagement/src/EventManagement_Cfg.su

.PHONY: clean-Core-2f-EventManagement-2f-src

