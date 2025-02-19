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
	GIE = 0;
	
	if(INTF == 1){
		
		INTF = 0;
		myINTF ^= 1;		//^= toggle function
		
	}else if(TMR0IF == 1){
	
		TMR0IF = 0;
		myTMR0IF= 1;
		
	}
	
	GIE = 1;
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
	unsigned char counter;
	
	TRISB = 0XFF;
	TRISC = 0X00;
	
	PORTC = 0x00;

	OPTION_REG = 0XC4;
	
	INTF = 0;
	INTE = 1;
	
	TMR0IF = 0;
	TMR0IE = 1;
	
	GIE = 1;
	
	while(1){
		if(myINTF == 0){
			
			while(myINTF != 1){
			}

			
		}else if(counter == 0x09){
		
			counter == 0x00;
			
		}else{
		
			counter++;
			
		}
		delay(122); 		//122 is approx. 1second
		PORTC = counter;
	}
}