#include <xc.h>
#include<stdio.h> 
#include <math.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF 	
#pragma config CP = OFF

int show = 0;
int control = 0;

void delay(int cnt) { 
	while(cnt--); 
} 


void interrupt ISR(void)
{
	GIE = 0; 		// disable all unmasked interrupts (INTCON reg) 

	if(ADIF==1) 	// checks CCP1 interrupt flag 
	{
		ADIF=0;
		show = 1;
	}
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
	
}
	
void main(void) { 
	int DVAL = 0; 
	TRISA = 0xFF; 	// set all PORTA as input
	TRISB = 0x00; 	// set all PORTB as output 
	TRISC = 0x00;
	PORTB = 0x00; 	// all LEDs are off 
	PORTC = 0x00;
	
	ADCON1 = 0x80; 	// result: right Justified, clock: FOSC/2 
					// all ports in PORTA are analog 
					// VREF+: VDD, VREF-: VSS 
	ADCON0 = 0x41; 	// clock: FOSC/8, analog channel: AN0, 
					// A/D conversion: STOP, A/D module: ON 
	ADIF = 0;
	ADIE = 1;
	PEIE = 1;
	GIE = 1;
	delay(1000);
	GO = 1;

	int DVAL = 0;
	int temp1=0;		// for decimal
	int temp2=0;		// for whole number

	while(1) { // foreground routine 
		if(show) {
			show = 0;
			/* read result register */ 
			DVAL = ADRESH; 		// read ADRESH 
			DVAL = DVAL << 8; // move to correct position 
			DVAL = DVAL | ADRESL; // read ADRESL 			
			
			temp2 = round(DVAL / 205);			// get ones place (1024/5 = 204.8)
			temp1 = round((DVAL % 205) / 20.48);	// divide the remainder by 20 (204.8/10=20.48)	//temp 2 whole temp1 decimal
			
			if (temp1 == 0x0A) {					// check if decimal = 10 / 0x0A
				temp1 = 0;							// set to 0
				temp2++;							// increment to whole number
			}
	
			if(control){
				RC0=0;
				RC1=1;
				RC2=1;
				
				PORTB=temp2 & 0x0F;
			}
			else{
				RC0=1;
				RC1=0;
				RC2=0;
				
				PORTB=temp1 & 0x0F;
			}
			
			control = !control;
		//	temp2 = temp2 << 4;			// the decimal is in the lower nibble so whole number must shift left
		//	DVAL = temp2 | temp1;	// combine values	
		//	PORTB = DVAL;			// display valyes

			delay(1000);
			GO = 1; 			// restart A/D Conversion
		}
	
	} 
} 
