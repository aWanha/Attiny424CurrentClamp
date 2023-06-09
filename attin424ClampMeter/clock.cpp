#include "IncFile1.h"

void init_clock(){

	CCP = 0xD8; //unlock write protection
	
	CLKCTRL_MCLKCTRLB = 0; //clock 20 MHz
	
}