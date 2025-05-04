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

void delay(int cnt) {
    while(cnt--){}
}

void USART_tx(char ch){
	while(!TXIF);
	TXREG=ch;
}

void ANDAM_tx(const unsigned char* msg){
	int i;
	while(*msg != '\0'){
		USART_tx(*msg++);
	}
	USART_tx('\r');
	USART_tx('\n');

}

void main(void)
{
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
	// (see formula in Table 10-1)
	SYNC = 0; // asynchronous mode (TXSTA reg)
	SPEN = 1; // enable serial port (RCSTA reg)
	TX9 = 0; // 8-bit transmission (TXSTA reg)
	BRGH = 1; // asynchronous high-speed (TXSTA reg)
	//CREN=1;	//enable continuous receive (RCSTA reg)
	TXEN = 1; // transmit enable (TXSTA reg)
	PEIE = 1;
	GIE = 1;
	
	TRISB = 0xFF; // set all ports in PORTB to output
	
	int DATA;
	while(1) // foreground routine
	{
		while(RB4){
			DATA=PORTB&0x0F;
			switch(DATA){
				case 0x00:
					ANDAM_tx("You pressed 1");
					break;
				case 0x01:
					ANDAM_tx("You pressed 2");
					break;
				case 0x02:
					ANDAM_tx("You pressed 3");
					break;
				case 0x04:
					ANDAM_tx("You pressed 4");
					break;
				case 0x05:
					ANDAM_tx("You pressed 5");
					break;
				case 0x06:
					ANDAM_tx("You pressed 6");
					break;
				case 0x08:
					ANDAM_tx("You pressed 7");
					break;
				case 0x09:
					ANDAM_tx("You pressed 8");
					break;	
				case 0x0A:
					ANDAM_tx("You pressed 9");
					break;
				case 0x0C:
					ANDAM_tx("You pressed *");
					break;
				case 0x0D:
					ANDAM_tx("You pressed 0");
					break;
				case 0x0E:
					ANDAM_tx("You pressed #");
					break;	
			}
			delay(30000);
		}
	}
}
