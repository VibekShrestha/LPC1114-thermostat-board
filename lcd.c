// program to control LCD
#include <LPC11xx.h>
#include "lcd.h"


int main()
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6)|(1<<16);	//enable the clock to GPIO and IOCON block respectivel	
	
	initLCD();									//initialize LCD
	backLightOn();
	sendString("VIBEK SHRESTHA");
	sendCommand(0xC0);
	sendString("SHRESTHA VIBEK");
	
	return 0;
}









