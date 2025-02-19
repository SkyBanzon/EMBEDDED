#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned char num;	//value of keypress
bit myINTF = 0;
bit myTMR0IF = 0;


void interrupt ISR(){
	GIE = 0; // disables all unmasked interrupts to prevent interrupt overlap
	if (INTF){ // check the interrupt flag

		INTF = 0;
		myINTF ^= 1;		//^= toggle function
		
	}else if(TMR0IF == 1){
	
		TMR0IF = 0;
		
		if(RB0 == 1){
			num = PORTD & 0x0F;
			
			switch(num){
				case 0x00: PORTC = 0x01; break;
				case 0x01: PORTC = 0x02; break;
				case 0x02: PORTC = 0x03; break;
				case 0x04: PORTC = 0x04; break;
				case 0x05: PORTC = 0x05; break;
				case 0x06: PORTC = 0x06; break;
				case 0x08: PORTC = 0x07; break;
				case 0x09: PORTC = 0x08; break;
				case 0x0A: PORTC = 0x07; break;
				case 0x0D: PORTC = 0x00; break;
				default: PORTC = 0x00;
			}
			myTMR0IF=1;
		}

	}
	 GIE = 1; // enable interrupts again
}


void delay(int count){
	int of_count = 0;
		
	while(of_count < count){
		if(myTMR0IF == 1){
			myTMR0IF = 0;
			of_count++;
			
		}
	}
}



void main(){
						//set ports if input/output
	TRISA = 0x00;
	TRISB = 0xFF;
	TRISC = 0x00;
	TRISD = 0xFF;

	OPTION_REG = 0x04; 	///1:32 prescaler

	T0IE = 1; 	
	TMR0IF = 0; 	

	GIE = 1; 	
	
	PORTA = 0x00;		// set port a and c off on start
	PORTC = 0x00;

	while (1)	{
		PORTA = 0x01;   //blinks LED
		delay(122);
		PORTA = 0x00;
		delay(122);		
	}
}
