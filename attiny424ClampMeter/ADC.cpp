#include "IncFile1.h"

void init_ADC(){
	
	ADC0_CTRLA |= ADC_ENABLE_bm;
	
	ADC0_CTRLB = ADC_PRESC_DIV16_gc;
	
	ADC0_CTRLC |= ADC_REFSEL_1024MV_gc; 
	
	ADC0_CTRLF = ADC_SAMPNUM_ACC1024_gc;
	
	ADC0_CTRLE |= 3; //sampling duration addition in adc clockcycles
	
	ADC0_PGACTRL = ADC_PGAEN_bm | ADC_GAIN_1X_gc;
	
	ADC0_MUXPOS = ADC_MUXPOS_AIN4_gc | ADC_VIA_PGA_gc;
	
	ADC0_MUXNEG = ADC_MUXPOS_AIN5_gc | ADC_VIA_PGA_gc;
	
	ADC0_COMMAND |= ADC_MODE_BURST_SCALING_gc | ADC_DIFF_bm;
	
}

uint32_t abs(int32_t value){
	
	if(value < 0) return -value; else return value;
}

void ADC_setRef(uint8_t setting){

	ADC0_CTRLC &= ~0x07;
	
	ADC0_CTRLC |= setting;
	
	_delay_ms(20);
	
}


void ADC_setGain(uint8_t gain){//0-4 values
	
	if(gain > 4) gain = 4;
	
	ADC0_PGACTRL = (gain << 5) | ADC_PGAEN_bm;
	
	_delay_ms(50);
}

int16_t ADC_Read(uint16_t sampleN){//singe sampling run takes ~13.8ms
	
		if(sampleN == 0) return 0;
			
		int32_t total = 0;
	
		for(uint16_t i = 0; i < sampleN; i++){
	
			ADC0_COMMAND |= ADC_START_IMMEDIATE_gc;
			
			while( !( ADC0_INTFLAGS & ADC_RESRDY_bm ) ){};
			
			total += (int16_t)ADC0_RESULT - adc_offset_bits;
			
			ADC0_INTFLAGS |= ADC_RESRDY_bm;
			
		}
		
		return (total / sampleN) / (1 << discardNBits ); //discard 8 bits, 16-> 8 bit resolution, not directly bitshifted since handling signed integer.
}