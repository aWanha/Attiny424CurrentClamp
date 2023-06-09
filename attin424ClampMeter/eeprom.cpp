#include "IncFile1.h"

uint8_t ee_read(uint8_t addr){ 
	
	return *(volatile uint8_t*)(EEPROM_START+addr);
}


void ee_write(uint8_t addr, uint8_t data){
	
	if( ee_read(addr) == data ) return; //return if stored data is the same
	
	while( NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm ); 
	
	*(volatile uint8_t*)(EEPROM_START + addr) = data;
	
	CCP = CCP_SPM_gc; 
	
	NVMCTRL.CTRLA = NVMCTRL_CMD_PAGEERASEWRITE_gc;
}

