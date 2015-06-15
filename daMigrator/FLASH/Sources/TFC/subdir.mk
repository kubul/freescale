################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/TFC/TFC.c" \
"../Sources/TFC/TFC_ADC.c" \
"../Sources/TFC/TFC_BoardSupport.c" \
"../Sources/TFC/TFC_CrystalClock.c" \
"../Sources/TFC/TFC_DMA.c" \
"../Sources/TFC/TFC_LineScanCamera.c" \
"../Sources/TFC/TFC_Motor.c" \
"../Sources/TFC/TFC_Queue.c" \
"../Sources/TFC/TFC_Servo.c" \
"../Sources/TFC/TFC_SysTick.c" \
"../Sources/TFC/TFC_Terminal.c" \
"../Sources/TFC/TFC_UART.c" \
"../Sources/TFC/TFC_arm_cm4.c" \
"../Sources/TFC/TFC_i2c.c" \

C_SRCS += \
../Sources/TFC/TFC.c \
../Sources/TFC/TFC_ADC.c \
../Sources/TFC/TFC_BoardSupport.c \
../Sources/TFC/TFC_CrystalClock.c \
../Sources/TFC/TFC_DMA.c \
../Sources/TFC/TFC_LineScanCamera.c \
../Sources/TFC/TFC_Motor.c \
../Sources/TFC/TFC_Queue.c \
../Sources/TFC/TFC_Servo.c \
../Sources/TFC/TFC_SysTick.c \
../Sources/TFC/TFC_Terminal.c \
../Sources/TFC/TFC_UART.c \
../Sources/TFC/TFC_arm_cm4.c \
../Sources/TFC/TFC_i2c.c \

OBJS += \
./Sources/TFC/TFC.o \
./Sources/TFC/TFC_ADC.o \
./Sources/TFC/TFC_BoardSupport.o \
./Sources/TFC/TFC_CrystalClock.o \
./Sources/TFC/TFC_DMA.o \
./Sources/TFC/TFC_LineScanCamera.o \
./Sources/TFC/TFC_Motor.o \
./Sources/TFC/TFC_Queue.o \
./Sources/TFC/TFC_Servo.o \
./Sources/TFC/TFC_SysTick.o \
./Sources/TFC/TFC_Terminal.o \
./Sources/TFC/TFC_UART.o \
./Sources/TFC/TFC_arm_cm4.o \
./Sources/TFC/TFC_i2c.o \

C_DEPS += \
./Sources/TFC/TFC.d \
./Sources/TFC/TFC_ADC.d \
./Sources/TFC/TFC_BoardSupport.d \
./Sources/TFC/TFC_CrystalClock.d \
./Sources/TFC/TFC_DMA.d \
./Sources/TFC/TFC_LineScanCamera.d \
./Sources/TFC/TFC_Motor.d \
./Sources/TFC/TFC_Queue.d \
./Sources/TFC/TFC_Servo.d \
./Sources/TFC/TFC_SysTick.d \
./Sources/TFC/TFC_Terminal.d \
./Sources/TFC/TFC_UART.d \
./Sources/TFC/TFC_arm_cm4.d \
./Sources/TFC/TFC_i2c.d \

OBJS_QUOTED += \
"./Sources/TFC/TFC.o" \
"./Sources/TFC/TFC_ADC.o" \
"./Sources/TFC/TFC_BoardSupport.o" \
"./Sources/TFC/TFC_CrystalClock.o" \
"./Sources/TFC/TFC_DMA.o" \
"./Sources/TFC/TFC_LineScanCamera.o" \
"./Sources/TFC/TFC_Motor.o" \
"./Sources/TFC/TFC_Queue.o" \
"./Sources/TFC/TFC_Servo.o" \
"./Sources/TFC/TFC_SysTick.o" \
"./Sources/TFC/TFC_Terminal.o" \
"./Sources/TFC/TFC_UART.o" \
"./Sources/TFC/TFC_arm_cm4.o" \
"./Sources/TFC/TFC_i2c.o" \

C_DEPS_QUOTED += \
"./Sources/TFC/TFC.d" \
"./Sources/TFC/TFC_ADC.d" \
"./Sources/TFC/TFC_BoardSupport.d" \
"./Sources/TFC/TFC_CrystalClock.d" \
"./Sources/TFC/TFC_DMA.d" \
"./Sources/TFC/TFC_LineScanCamera.d" \
"./Sources/TFC/TFC_Motor.d" \
"./Sources/TFC/TFC_Queue.d" \
"./Sources/TFC/TFC_Servo.d" \
"./Sources/TFC/TFC_SysTick.d" \
"./Sources/TFC/TFC_Terminal.d" \
"./Sources/TFC/TFC_UART.d" \
"./Sources/TFC/TFC_arm_cm4.d" \
"./Sources/TFC/TFC_i2c.d" \

OBJS_OS_FORMAT += \
./Sources/TFC/TFC.o \
./Sources/TFC/TFC_ADC.o \
./Sources/TFC/TFC_BoardSupport.o \
./Sources/TFC/TFC_CrystalClock.o \
./Sources/TFC/TFC_DMA.o \
./Sources/TFC/TFC_LineScanCamera.o \
./Sources/TFC/TFC_Motor.o \
./Sources/TFC/TFC_Queue.o \
./Sources/TFC/TFC_Servo.o \
./Sources/TFC/TFC_SysTick.o \
./Sources/TFC/TFC_Terminal.o \
./Sources/TFC/TFC_UART.o \
./Sources/TFC/TFC_arm_cm4.o \
./Sources/TFC/TFC_i2c.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/TFC/TFC.o: ../Sources/TFC/TFC.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_ADC.o: ../Sources/TFC/TFC_ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_ADC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_BoardSupport.o: ../Sources/TFC/TFC_BoardSupport.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_BoardSupport.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_BoardSupport.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_CrystalClock.o: ../Sources/TFC/TFC_CrystalClock.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_CrystalClock.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_CrystalClock.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_DMA.o: ../Sources/TFC/TFC_DMA.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_DMA.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_DMA.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_LineScanCamera.o: ../Sources/TFC/TFC_LineScanCamera.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_LineScanCamera.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_LineScanCamera.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_Motor.o: ../Sources/TFC/TFC_Motor.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_Motor.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_Motor.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_Queue.o: ../Sources/TFC/TFC_Queue.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_Queue.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_Queue.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_Servo.o: ../Sources/TFC/TFC_Servo.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_Servo.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_Servo.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_SysTick.o: ../Sources/TFC/TFC_SysTick.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_SysTick.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_SysTick.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_Terminal.o: ../Sources/TFC/TFC_Terminal.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_Terminal.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_Terminal.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_UART.o: ../Sources/TFC/TFC_UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_UART.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_UART.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_arm_cm4.o: ../Sources/TFC/TFC_arm_cm4.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_arm_cm4.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_arm_cm4.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TFC/TFC_i2c.o: ../Sources/TFC/TFC_i2c.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TFC/TFC_i2c.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TFC/TFC_i2c.o"
	@echo 'Finished building: $<'
	@echo ' '


