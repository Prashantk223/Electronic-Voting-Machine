################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.c \
../Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.c \
../Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.c \
../Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.c 

OBJS += \
./Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.o \
./Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.o \
./Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.o \
./Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.o 

C_DEPS += \
./Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.d \
./Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.d \
./Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.d \
./Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/HAL_Drivers/src/%.o Drivers/HAL_Drivers/src/%.su Drivers/HAL_Drivers/src/%.cyclo: ../Drivers/HAL_Drivers/src/%.c Drivers/HAL_Drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/BSP/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/EventManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Core/ModeManagement/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Drivers/HAL_Drivers/inc" -I"C:/Embedded/BareMetalDriverDevelopment/Projects/MCU1/EVM_Project/Middleware/Schedular/CoperativeSchedular/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-HAL_Drivers-2f-src

clean-Drivers-2f-HAL_Drivers-2f-src:
	-$(RM) ./Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.cyclo ./Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.d ./Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.o ./Drivers/HAL_Drivers/src/stm32f407xx_gpio_driver.su ./Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.cyclo ./Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.d ./Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.o ./Drivers/HAL_Drivers/src/stm32f407xx_rcc_driver.su ./Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.cyclo ./Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.d ./Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.o ./Drivers/HAL_Drivers/src/stm32f407xx_timer_driver.su ./Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.cyclo ./Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.d ./Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.o ./Drivers/HAL_Drivers/src/stm32f407xx_usart_driver.su

.PHONY: clean-Drivers-2f-HAL_Drivers-2f-src

