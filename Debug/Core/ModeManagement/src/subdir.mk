################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ModeManagement/src/ModeManagement.c 

OBJS += \
./Core/ModeManagement/src/ModeManagement.o 

C_DEPS += \
./Core/ModeManagement/src/ModeManagement.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ModeManagement/src/%.o Core/ModeManagement/src/%.su Core/ModeManagement/src/%.cyclo: ../Core/ModeManagement/src/%.c Core/ModeManagement/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/EventManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/ModeManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/HAL_Drivers/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Middleware/Schedular/CoperativeSchedular/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/BSP/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-ModeManagement-2f-src

clean-Core-2f-ModeManagement-2f-src:
	-$(RM) ./Core/ModeManagement/src/ModeManagement.cyclo ./Core/ModeManagement/src/ModeManagement.d ./Core/ModeManagement/src/ModeManagement.o ./Core/ModeManagement/src/ModeManagement.su

.PHONY: clean-Core-2f-ModeManagement-2f-src

