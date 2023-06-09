/*
 * IncFile1.h
 *
 * Created: 24.5.2023 8.24.30
 *  Author: home
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#define USART1_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define F_CPU 20000000

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/eeprom.h>
#define USART1_newline USART1_sendStr("\r\n")

constexpr int16_t adc_correction_uV = 8656;
constexpr int16_t adc_offset_bits = 280;
constexpr uint16_t pwmResolution = 5000;
constexpr uint8_t discardNBits = 8; //amount of noise in oversampled 16 bit result, mA resolution could be improved by keeping more bits.

constexpr int16_t zeropointOffset = 0; //move initial setpoint where offset nulling will aim to.

void init_clock();

void init_pins();

void init_ADC();

void init_RTC();

void init_TimerA();

void Init_USART1();

int16_t ADC_Read(uint16_t sampleN);

void ADC_setGain(uint8_t gain);

void USART1_sendChar(char c);

void USART1_sendStr(const char *str);

char USART1_receive();

void USART1_sendInt(int32_t a, uint8_t minStrSize);

uint32_t USART1_toValue(const char *str);

void setPwmDutycycle(uint16_t &dutyCycle);

uint32_t abs(int32_t value);

void nullOffset(uint16_t &dutyCycle, int16_t range);

uint8_t noiseBits(uint8_t sampleN);

void ADC_setRef(uint8_t setting);

int16_t resultCorrectionmV(int32_t average, uint8_t gain);

uint8_t ee_read(uint8_t addr);

void ee_write(uint8_t addr, uint8_t data);

#endif /* INCFILE1_H_ */