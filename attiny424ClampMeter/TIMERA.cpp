/*
 * TIMERA.cpp
 *
 * Created: 24.1.2023 10.59.49
 *  Author: home
 */ 

#include "IncFile1.h"

void init_TimerA(){
	
	TCA0_SINGLE_CTRLA |= TCA_SINGLE_ENABLE_bm;
	
	TCA0_SINGLE_CTRLB |= TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	
	TCA0_SINGLE_PER = pwmResolution;
	
	PORTMUX_TCAROUTEA = PORTMUX_TCA00_ALT1_gc;
	
	PORTB_DIRSET = PIN3_bm;
	
}

void setPwmDutycycle(uint16_t &dutyCycle){
	
	TCA0_SINGLE_CMP0BUF = dutyCycle;
	
}