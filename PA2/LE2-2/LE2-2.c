#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void main()
{
	unsigned char data;
	OPTION_REG = 0xC0; // configuring the OPTION register
	TRISA = 0x00; // set all of PORTB as output    LEDs
	TRISC = 0x00; // set ALL PORT C TO OUTPUT	   LCD INIT
	TRISB = 0x00; // set all of PORTB as output    LCD DISPLAY
	TRISD = 0X1F; //SET PORTD AS INPUT              KEYPAD INPUT

	while(RD4){
		data=PORTD&0X0F;

		switch(data){
		  case 0X00:
		    PORTA=0X01;
		    break;
		  case 0X01:
		    PORTA=0X02;
		    break;
		  case 0X02:
		    PORTA=0X03;
		    break;
		  case 0X04:
		    PORTA=0X04;
		    break;
		  case 0X05:
		    PORTA=0X05;
		    break;
		  case 0X06:
		    PORTA=0X06;
		    break;
		  case 0X08:
		    PORTA=0X07;
		    break;
		  case 0X09:
		    PORTA=0X08;
		    break;
		  case 0X0A:
		    PORTA=0X09;
		    break;
		  case 0X0C:
		    PORTA=0X0F;
		    break;
		  case 0X0D:
		    PORTA=0X00;
		    break;
		  case 0X0E:
		    PORTA=0X0F;
		    break;
		  default:
		    PORTA=0X00;
		    break;
			
		}
	}


}
