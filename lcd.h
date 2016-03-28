#define __lcd_h

#include <lpc11xx.h>

//****assigning name to the pins****//
#define led 			 R_PIO1_1	
#define ledPin  	 1
#define buz 			 PIO1_5
#define buzPin		 5
#define lcdBl 		 R_PIO1_0
#define lcdBlPin 	 0
#define rs				 PIO1_9
#define rsPin			 9
#define en				 PIO2_4
#define enPin			 4
#define rw				 PIO3_4
#define rwPin			 4
#define d4				 PIO0_8
#define d4Pin			 8
#define d5				 PIO0_9
#define d5Pin			 9
#define d6				 SWCLK_PIO0_10
#define d6Pin			 10
#define d7				 R_PIO0_11
#define d7Pin			 11
//****assigning name to the pins****//

void delay(void)
{
	unsigned int i=0;
	for(i=0;i<=0x1fff;i++);
}

void backLightOn()		//call for backlight on
{
	LPC_GPIO1->DATA |=(1<<lcdBlPin);
}

void backLightOff()		//call for backlight off
{
	LPC_GPIO1->DATA &=~(1<<lcdBlPin);
}

void configurePorts()	//call for configuring pinouts
{
	LPC_IOCON->d4 &=~(0x10);					//LCD D0 //pull up register disabled
	LPC_IOCON->d5 &=~(0x10);					//LCD D1 //pull up register disabled
	LPC_IOCON->d6 |=(0x01); 		//register set to GPIO mode
	LPC_IOCON->d6 &=~(0x10);		//LCD D2 //pull up register disabled
	LPC_IOCON->d7 |=(0x01);				//register set to GPIO
	LPC_IOCON->d7 &=~(0x10);				//LCD D3 //pull up register disabled
	LPC_IOCON->rs &=~(0x10);					//LCD RS //pull up register disabled
	LPC_IOCON->rw &=~(0x10);					//LCD R/W //pull up register disabled
	LPC_IOCON->en &=~(0x10);					//LCD EN //pull up register disabled
	LPC_IOCON->lcdBl |=(0x01);					//register set to GPIO 
	LPC_IOCON->lcdBl &=~(0x10);				//LCD backlight //pull up register disabled
	
	//LPC_GPIO0->DIR =0xf00;	//set d4, d5, d6, d7 as output.
	LPC_GPIO0->DIR |=(1<<d4Pin);	//set d4 to output
	LPC_GPIO0->DIR |=(1<<d5Pin);	//set d5 to output
	LPC_GPIO0->DIR |=(1<<d6Pin);	//set d6 to output
	LPC_GPIO0->DIR |=(1<<d7Pin);	//set d7 to output
	LPC_GPIO1->DIR |=(1<<rsPin);	//set rs to output
  LPC_GPIO2->DIR |=(1<<rwPin);	//set rw to output
  LPC_GPIO3->DIR |=(1<<enPin);	//set en to output	
	LPC_GPIO1->DIR |=(1<<lcdBlPin);	//set direction of lcd backlight to output
}

void dataRead()			//call for read pulse trigger
{
	LPC_GPIO2->DATA &=~(1<<enPin);	//make enable pin low to read
	delay();
	LPC_GPIO2->DATA |=(1<<enPin);		//make enable pin high to write
}

void checkBusy()		//call for lcd busy flag check
{
	LPC_GPIO1->DIR &=~(1<<d7Pin); //set d7 to input
	while((LPC_GPIO1->DATA & (1<<d7Pin))==1)
	{
		dataRead();
	}
	LPC_GPIO1->DIR |=(1<<d7Pin);  //set d7 to output
}

void dataWrite()		//call for write pulse trigger
{
	LPC_GPIO2->DATA |=(1<<enPin);		//make enable pin high to write
	delay();
	LPC_GPIO2->DATA &=~(1<<enPin);	//make enable pin low to read
}

void sendCommand(unsigned char cmd)	//call for lcd preference set
{
	checkBusy();
	LPC_GPIO3->DATA &=~(1<<rwPin);	//set to write mode
	LPC_GPIO1->DATA &=~(1<<rsPin);	//set to command register
	LPC_GPIO0->DATA =(0xF0 & cmd)<<4;  //set higher nibble
	dataWrite(); //give enable trigger
	LPC_GPIO0->DATA =(0x0F & cmd)<<8;	//set lower nibble
	dataWrite(); //give enable trigger
}

void initLCD()			//call for lcd initialize sequence
{
	configurePorts();						//configure all required pins for LCD
	
	sendCommand(0x33);	//lcd initialize sequence 1
	sendCommand(0x32);	//lcd initialize sequence 2
	sendCommand(0x28);	//4 bit mode
	sendCommand(0x0E);	//clear the screen
	sendCommand(0x01);	//display on cursor on
	sendCommand(0x06);	//increment cursor
	sendCommand(0x80);	//row 1 column 1
}

void sendData(unsigned char data)		//call for writing data to display
{
	checkBusy();
	LPC_GPIO0->DATA =0x000;
	LPC_GPIO3->DATA &=~(1<<rwPin);	//set to write mode
	LPC_GPIO1->DATA |=(1<<rsPin);	//set to command register
	LPC_GPIO0->DATA =(0xF0 & data)<<4;  //set higher nibble
	dataWrite(); //give enable trigger
	LPC_GPIO0->DATA =(0x0f & data)<<8;	//set lower nibble
	dataWrite(); //give enable trigger
}

void sendString(char *s)			//call for string processing
{
	while(*s)
	{
		sendData(*s);
		s++;
	}
}

