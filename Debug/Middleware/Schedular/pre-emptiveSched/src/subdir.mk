################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/Schedular/pre-emptiveSched/src/task.c 

OBJS += \
./Middleware/Schedular/pre-emptiveSched/src/task.o 

C_DEPS += \
./Middleware/Schedular/pre-emptiveSched/src/task.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/Schedular/pre-emptiveSched/src/%.o Middleware/Schedular/pre-emptiveSched/src/%.su Middleware/Schedular/pre-emptiveSched/src/%.cyclo: ../Middleware/Schedular/pre-emptiveSched/src/%.c Middleware/Schedular/pre-emptiveSched/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/EventManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/ModeManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/HAL_Drivers/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Middleware/Schedular/CoperativeSchedular/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/BSP/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middleware-2f-Schedular-2f-pre-2d-emptiveSched-2f-src

clean-Middleware-2f-Schedular-2f-pre-2d-emptiveSched-2f-src:
	-$(RM) ./Middleware/Schedular/pre-emptiveSched/src/task.cyclo ./Middleware/Schedular/pre-emptiveSched/src/task.d ./Middleware/Schedular/pre-emptiveSched/src/task.o ./Middleware/Schedular/pre-emptiveSched/src/task.su

.PHONY: clean-Middleware-2f-Schedular-2f-pre-2d-emptiveSched-2f-src

