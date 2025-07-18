################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/Schedular/PreemptiveSched/src/task.c 

OBJS += \
./Middleware/Schedular/PreemptiveSched/src/task.o 

C_DEPS += \
./Middleware/Schedular/PreemptiveSched/src/task.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/Schedular/PreemptiveSched/src/%.o Middleware/Schedular/PreemptiveSched/src/%.su Middleware/Schedular/PreemptiveSched/src/%.cyclo: ../Middleware/Schedular/PreemptiveSched/src/%.c Middleware/Schedular/PreemptiveSched/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/EventManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/ModeManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/HAL_Drivers/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Middleware/Schedular/CoperativeSchedular/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/BSP/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middleware-2f-Schedular-2f-PreemptiveSched-2f-src

clean-Middleware-2f-Schedular-2f-PreemptiveSched-2f-src:
	-$(RM) ./Middleware/Schedular/PreemptiveSched/src/task.cyclo ./Middleware/Schedular/PreemptiveSched/src/task.d ./Middleware/Schedular/PreemptiveSched/src/task.o ./Middleware/Schedular/PreemptiveSched/src/task.su

.PHONY: clean-Middleware-2f-Schedular-2f-PreemptiveSched-2f-src

