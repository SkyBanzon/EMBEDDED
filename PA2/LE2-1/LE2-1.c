#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay (unsigned char MUL)
{
	int i,j;
	for (i=MUL; i != 0; i--) // loop until i = 0
	{
		for (j=0; j < 256; j++); // loop until j = 0
		{
		}
	}
}

void instCtrl (unsigned char INST)
{
	PORTB = INST; // load instruction to PORTB
	RC0 = 0; // set RS to 0 (instruction reg)
	RC1 = 1; // set E to 1
	RC2 = 0; // set RW to 0 (write)
	delay(1); // 1 ms delay (estimate)
	RC1 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTB = DATA; // load data to PORTB
	RC0 = 1; // set RS to 1 (data reg)
	RC1 = 1; // set E to 1
	RC2 = 0; // set RW to 0 (write)
	delay(1); // 1 ms delay (estimate)
	RC1 = 0; // set E to 0 (strobe)
}


void initLCD()
{
	delay(1);
	instCtrl(0x38);
	instCtrl(0x08);
	instCtrl(0x01);
	instCtrl(0x06);
	instCtrl(0x0F);
}


void main()
{
	unsigned char data;
	OPTION_REG = 0xC0; // configuring the OPTION register
	TRISC = 0x00; // set RA0-RA3 as input, the rest are output
	TRISB = 0x00; // set all of PORTB as output

	initLCD();
	instCtrl(0xC6); // move cursor to 2nd line 7th column
	dataCtrl('H'); // prints ‘H’ at current cursor position
					// then shifts the cursor to the right*
	dataCtrl('E'); // prints ‘E’
	dataCtrl('L'); // prints ‘L’
	dataCtrl('L'); // prints ‘L’
	dataCtrl('O'); // prints ‘O’
	dataCtrl('!'); // prints ‘!’

	while(1){
	}


}
