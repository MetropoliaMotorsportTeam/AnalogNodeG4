################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/config.c \
../Core/Src/curve.c \
../Core/Src/flash_conf.c \
../Core/Src/functions.c \
../Core/Src/main.c \
../Core/Src/sensors.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/transfer_functions.c \
../Core/Src/virtual_sensors.c 

OBJS += \
./Core/Src/config.o \
./Core/Src/curve.o \
./Core/Src/flash_conf.o \
./Core/Src/functions.o \
./Core/Src/main.o \
./Core/Src/sensors.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/transfer_functions.o \
./Core/Src/virtual_sensors.o 

C_DEPS += \
./Core/Src/config.d \
./Core/Src/curve.d \
./Core/Src/flash_conf.d \
./Core/Src/functions.d \
./Core/Src/main.d \
./Core/Src/sensors.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/transfer_functions.d \
./Core/Src/virtual_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/config.cyclo ./Core/Src/config.d ./Core/Src/config.o ./Core/Src/config.su ./Core/Src/curve.cyclo ./Core/Src/curve.d ./Core/Src/curve.o ./Core/Src/curve.su ./Core/Src/flash_conf.cyclo ./Core/Src/flash_conf.d ./Core/Src/flash_conf.o ./Core/Src/flash_conf.su ./Core/Src/functions.cyclo ./Core/Src/functions.d ./Core/Src/functions.o ./Core/Src/functions.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/sensors.cyclo ./Core/Src/sensors.d ./Core/Src/sensors.o ./Core/Src/sensors.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/transfer_functions.cyclo ./Core/Src/transfer_functions.d ./Core/Src/transfer_functions.o ./Core/Src/transfer_functions.su ./Core/Src/virtual_sensors.cyclo ./Core/Src/virtual_sensors.d ./Core/Src/virtual_sensors.o ./Core/Src/virtual_sensors.su

.PHONY: clean-Core-2f-Src

