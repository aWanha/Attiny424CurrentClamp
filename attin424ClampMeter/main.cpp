/*
 * attin424ClampMeter.cpp
 *
 * Created: 28.5.2023 22.14.04
 * Author : home
 */ 

#include "IncFile1.h"

int main(void)
{
	
	bool offsetZeroFlag = true;
	bool initialSetup = false;
	bool lowRange = true;
	
	const int16_t sampleListSize = 20;
	int16_t sampleList[sampleListSize];
	uint8_t sampleList_index = 0;
	
	int16_t offset;
	int16_t result;

	uint16_t dutyCycle = ( (uint16_t)ee_read(0) << 8) + (uint16_t)ee_read(1); //read last offset value from eeprom
	
	if(dutyCycle == 0xFFFF)dutyCycle = pwmResolution >> 1; //if eeprom is empty, use default 50% dutycycle

	int32_t average = 0;
	
	uint8_t sampleN = 3; //how many adc cycles to perform when sampling. 1 ADC cycle already oversampling 1024 times.
	
	init_clock();
	Init_USART1();
	init_TimerA();
	init_ADC();
	init_pins();
	
	USART1_sendStr("initialized\r\n");

	ADC_setGain(0);

    while (1) 
    {
		//interface actions---------------
		
		if(PORTB_INTFLAGS & PIN2_bm){ //button 1 for offset null, use interrupt flags to detect button press
			
			offsetZeroFlag = 1;
			
		}
		if(PORTA_INTFLAGS & PIN6_bm || !initialSetup){ //button 2 for range selection
			
			lowRange = !lowRange;
			
			if(lowRange){
				
				USART1_sendStr("low range, max 750mA\r\n");
				
			}else{
				
				USART1_sendStr("high range, max 6A\r\n");
			}
						
			ADC_setGain(2 << lowRange); //best correction curves for gain settings of 2x (value 2) and 16x (value 4)
			
			offsetZeroFlag = 1;

		}
		
		if(offsetZeroFlag){
			
			nullOffset(dutyCycle,800 >> discardNBits);//aim offset to desired zeropoint within range of 2%, 800/32768

			if(!initialSetup){//initial ramp of pwm signal will take several seconds to reach final value, starting firt time add longer delay and store offset value to eeprom.
				
				initialSetup = true;
				USART1_sendStr("initial setup, 5 sec..\r\n");
				
				uint16_t lastsetting = ( (uint16_t)ee_read(0) << 8) + (uint16_t)ee_read(1);
				
				if( !( (dutyCycle - 200 < lastsetting ) && (lastsetting < dutyCycle + 200) ) ){ //store new dutycycle value in eeprom if new setting is not within 200 range of last one.
					
					ee_write(0, dutyCycle >> 8); //16 bit value split into 2 bytes
					
					_delay_ms(10);
					
					ee_write(1, dutyCycle & 0xFF);
					
				}
				
				 _delay_ms(5000);//long delay before measuring if this was initial setup
				
			}
			USART1_sendStr("offset zero\r\n");
			
			_delay_ms(5); //small settling time if usart sending disrupts reference voltage.
			
			offset = ADC_Read(128); //large window of measuring to get average of varying signal(50Hz and mcuh lower frequencies can be seen if more adc bits are used)
			
			//reset values 
					
			for(uint16_t i = 0; i < sampleListSize; i++){ //clear average list
							
				sampleList[i] = 0;
			}
			
			offsetZeroFlag = false;

			average = 0;
			
			PORTB_INTFLAGS = PIN2_bm;
			PORTA_INTFLAGS = PIN6_bm;
		}
		
		//normal operation loop----------------------------
		
		result = ADC_Read(sampleN);
		
		if(abs(result) >= (0x7FFF >> discardNBits) - 2 )USART1_sendStr("ovf\r\n"); //overflow indicator
		
		else{
			
		result = result-offset;
	
		average -= sampleList[sampleList_index]; //circle list of samples, oldest one replaced with new one
		
		sampleList[sampleList_index] = result; //new sample
		
		average += sampleList[sampleList_index];//new average
		
		if(sampleList_index < sampleListSize - 1) sampleList_index +=1; else sampleList_index = 0; //circular loop
		
		USART1_sendStr("current: ");
		
		USART1_sendInt( resultCorrectionmV( ((int32_t)result * sampleListSize),  (2 << lowRange) ), 4); //rapidly changing result
		
		USART1_sendStr("mA, currentAverage: ");
		USART1_sendInt( resultCorrectionmV( ((int32_t)average),  (2 << lowRange) ), 4); //average result

		USART1_sendStr("mA, \r\n");
		}
		
		_delay_ms(5); //delay to avoid usart signals affecting reference voltage
    }
}

