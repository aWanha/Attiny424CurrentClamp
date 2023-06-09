#include "IncFile1.h"


void nullOffset(uint16_t &dutyCycle, int16_t range){
	
	int16_t result;
	
	uint16_t stepsize; //stepsize to give acceleration for pwm adjustment

	uint8_t refsel = 7; //calibration at highest reference to avoid saturating adc if reading exceed 1024mV reference.
	
	ADC_setRef(refsel); // calibration with 4096 mV refrence
	
	while(1){
		
		result = ADC_Read(10);
		
		stepsize = abs(result);

		stepsize = stepsize >> (9-discardNBits); // acceleration adjustment

		if(stepsize == 0) stepsize = 1;

		if(range - zeropointOffset < result){
			
			if(stepsize < dutyCycle)dutyCycle -= stepsize; // clamp to 0
			
			}else if(result < -zeropointOffset-range){
			
			if(dutyCycle < (pwmResolution-stepsize) )dutyCycle += stepsize; //clamp to max pwmResolution
			
			}else{ //if offset is within desired range, change reference to 1024mV and idle to give time for pwm filter settling
			
			ADC_setRef(4);
			_delay_ms(400);
			
			return;
			}
			
		//debug
		setPwmDutycycle(dutyCycle);
		
		USART1_sendChar('d');
		USART1_sendInt(dutyCycle,0);
		USART1_sendChar('R');
		USART1_sendInt(result,0);

		USART1_newline;
		
	
		_delay_ms(50); //settling time for each adjustment iteration
	}
}

uint8_t noiseBits(uint8_t sampleN){//function to see how many bits are varying
	
	uint16_t result = abs(ADC_Read(sampleN)); //reference reading
	uint16_t noise;
	uint16_t avrnoise = 0;
	
	for(uint8_t j = 0; j < 16; j++){
		
		noise = result ^ abs(ADC_Read(sampleN)); //xor to see varying bits
		uint8_t i = 0;
		
		for(; 0 < noise ; i++){
			
			noise = noise >> 1; //find highest varying bit

		}
		avrnoise += i;
		_delay_ms(10);
	}
	return avrnoise >> 4; //average of 16 so division by 16.

}


int16_t resultCorrectionmV(int32_t average, uint8_t gain){ //function to translate read value into current reading
	
	bool sign = false;
	
	if(average == 0)return 0;
	
	if(average < 0) sign = true;
	
	double x = abs(average); //calculations without sign
	
	double correction = 0;
	
	switch(gain){ //best slopes in PGA gain setting 2 and 4.
		
		/*
		case 1:
		
		correction = -2.25060E-09 * x * x * x + 4.47640E-06 * x * x - 2.97466E-03 * x + 3.04349E+00;
		break;
		*/
		case 2:
		
		correction = 9.9415E-08 * x * x - 1.7782E-04 * x + 1.2359E+00;
		break;
		/*
		case 3:
		
		correction = -1.6933E-07 * x * x + 1.9436E-04 * x + 5.3795E-01;
		break;
		*/
		case 4:
		
		correction = 0.289317;
		break;
	}
	
	if(sign) correction = -correction;//return sign
	
	return x * correction;
}