################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/015uart_tx.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/015uart_tx.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/015uart_tx.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/EventManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/ModeManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/HAL_Drivers/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Middleware/Schedular/CoperativeSchedular/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/BSP/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/015uart_tx.cyclo ./Src/015uart_tx.d ./Src/015uart_tx.o ./Src/015uart_tx.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

