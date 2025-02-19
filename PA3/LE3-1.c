#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit myINTF = 0;
bit myTMR0IF = 0;


void interrupt ISR(){
	GIE = 0; // disables all unmasked interrupts to prevent interrupt overlap
	if (INTF){ // check the interrupt flag

		INTF = 0; // clears the interrupt flag
		if(PORTD)

	}
	 GIE = 1; // enable interrupts again
}

void delay(int count){
	int of_count = 0;
		
	while(of_count < count){
		if(myTMR0IF == 1){
			of_count++;
			myTMR0IF = 0;
		}
	}
}



void main(){
	
	TRISC = 0X00;
	TRISD = 0XFF;
	OPTION_REG = 0XC3;

	INTEDG = 1; // interrupt at rising edge
	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag

 	GIE = 1; // enables all unmasked interrupt

	while(1){
	}
}