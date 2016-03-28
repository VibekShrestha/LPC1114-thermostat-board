//control led blinking

#include <LPC11XX.h>
#include "blinky.h"

void delay(void)
{
	unsigned int i=0;
	for(i=0;i<0x9ffff;i++);
}
int main()
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6) | (1<<16);	//managing clock for GPIO and IOCON
	LPC_IOCON->led |=(0x01);	//set led pin to GPIO mode
	LPC_IOCON->led &=~(0x10);	//disable pullup register
	LPC_GPIO1->DIR	|=(1<<ledPin);	//set led pin to output
	while(1)
	{
		LPC_GPIO1->DATA &=~(1<<ledPin);
		delay();
		LPC_GPIO1->DATA |=(1<<ledPin);
		delay();
	}
}
