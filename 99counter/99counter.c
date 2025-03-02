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
	GIE = 0; 	// disables all unmasked interrupts to prevent interrupt overlap
	if (INTF){  // check the interrupt flag

		INTF = 0; 	// clears the interrupt flag
		myINTF ^= 1;
	}
	else if(TMR0IF){
		TMR0IF = 0;
		myTMR0IF ^= 1;
	}
	 GIE = 1; // enable interrupts again
}

void delay(int overflow_count) {
    int of_count = 0;
    while (of_count < overflow_count) {
        if (myTMR0IF == 1) {
            of_count++;
            myTMR0IF = 0;
        }
    }
}

/*
void delay(unsigned char MUL)
{
	int i,j;
	for (i=MUL; i != 0; i--) // loop until i = 0
	{
		for (j=0; j < 256; j++); // loop until j = 0
		{
		}
	}
}

*/

void main(){
	
	TRISB = 0X0F;	//intr button
	TRISC = 0X00;	//7seg A
	TRISD = 0X00;	//7seg B
	OPTION_REG = 0XC4;
	
	INTEDG = 1;// RISING EDGE MMDMFMDFMHFGHMFGH
	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag
 	
	TMR0IE = 1;	//timer0 intr enable	dapat naay tmr0if sa imo ISR para mo gana
	TMR0IF = 0;	//timer0 intr flag =0

 	GIE = 1; // enables all unmasked interrupt

 	///////////////////////////////////////////////////////////
 	
	unsigned char LSB = 0x00;
	unsigned char MSB = 0x00;
	
	PORTC = 0x00;
	PORTD = 0x00;
	
	while(1){
		if(myINTF==0){
			if(LSB == 9){
				LSB = 0;
				if(MSB==9){
					MSB=0;
				}else{
					MSB++;
				}
			}
			else if(LSB != 9){
				LSB++;	
			}		
			delay(122);
			PORTD = LSB;
			PORTC = MSB;	
		}
		else if(myINTF){
			PORTD = LSB;
			PORTC = MSB;
		}
	}
}
