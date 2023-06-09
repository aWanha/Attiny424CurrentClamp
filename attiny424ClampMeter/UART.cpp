#include "IncFile1.h"

void Init_USART1(){
	
	
	USART1_BAUD = (uint16_t)USART1_BAUD_RATE(115200);
	
	USART1_CTRLB |= USART_TXEN_bm; //Enable Tx
	
	PORTA_DIRSET = PIN1_bm;
	
}

void USART1_sendChar(char c)
{
	while (!(USART1.STATUS & USART_DREIF_bm)){};

	USART1.TXDATAL = c;
}

void USART1_sendStr(const char *str){

	for(uint8_t i = 0; i<strlen(str); i++){

		USART1_sendChar(str[i]);
	}
	
	
}

char USART1_receive(){
	
	while (!(USART0_STATUS & 0x80)) {}
	
	return USART0.RXDATAL;
}

uint32_t USART1_toValue(const char *str) {
	uint32_t value = 0;
	uint8_t valueLength = strlen(str);
	uint32_t j = 1;
	
	if(valueLength>0){
		
		
		for (int8_t i = valueLength-1; i>=0; i--){
			
			if ((str[i] > 47 && str[i] < 58 )) { // is character digit?


				value += j * uint8_t(str[i] - '0'); //turn character to integer
				j *= 10;
			}
			else{
				
				value = 0; //non integer character found, return 0
				break;
			}
		}
	}
	return value;
}


void USART1_sendInt(int32_t a, uint8_t minStrSize){


	if(a<0){
		
		a = -a;
		USART1_sendChar('-');
		
		if(0 < minStrSize)minStrSize -=1;
		
	}

	char tempArray[10];

	uint8_t power = 10;
	uint8_t val;
	
	for(uint8_t j = 0; j<10; j++){

		if( a<1 )
		{
			tempArray[j] = '\0';
			break;
		}
		val = a  % power; //remainder is rightmost digit
		tempArray[j] = (char)( '0' + val ); // store ascii value
		
		a = a / 10; //remove zero

	}
	uint8_t arrayLen = strlen(tempArray);

	
	for (uint8_t j = 0;j<arrayLen>>1;j++){ //reverse char array

		char tempChar = tempArray[j];
		tempArray[j] = tempArray[arrayLen-j-1];
		tempArray[arrayLen-j-1] = tempChar;

	}

	for(; minStrSize > arrayLen; minStrSize--) USART1_sendChar('0');
	
	USART1_sendStr(tempArray);
}
