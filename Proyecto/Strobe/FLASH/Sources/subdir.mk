################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/LCD_lib.c" \
"../Sources/SCI_lib.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/LCD_lib.c \
../Sources/SCI_lib.c \
../Sources/main.c \

OBJS += \
./Sources/LCD_lib_c.obj \
./Sources/SCI_lib_c.obj \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/LCD_lib_c.obj" \
"./Sources/SCI_lib_c.obj" \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/LCD_lib_c.d \
./Sources/SCI_lib_c.d \
./Sources/main_c.d \

C_DEPS_QUOTED += \
"./Sources/LCD_lib_c.d" \
"./Sources/SCI_lib_c.d" \
"./Sources/main_c.d" \

OBJS_OS_FORMAT += \
./Sources/LCD_lib_c.obj \
./Sources/SCI_lib_c.obj \
./Sources/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/LCD_lib_c.obj: ../Sources/LCD_lib.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/LCD_lib.args" -ObjN="Sources/LCD_lib_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/SCI_lib_c.obj: ../Sources/SCI_lib.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/SCI_lib.args" -ObjN="Sources/SCI_lib_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


