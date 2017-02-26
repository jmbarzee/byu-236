################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DataLog.cpp \
../LexicalAnalyzer.cpp \
../Parameter.cpp \
../Parser.cpp \
../Predicate.cpp \
../Rule.cpp \
../Token.cpp \
../main.cpp 

OBJS += \
./DataLog.o \
./LexicalAnalyzer.o \
./Parameter.o \
./Parser.o \
./Predicate.o \
./Rule.o \
./Token.o \
./main.o 

CPP_DEPS += \
./DataLog.d \
./LexicalAnalyzer.d \
./Parameter.d \
./Parser.d \
./Predicate.d \
./Rule.d \
./Token.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


