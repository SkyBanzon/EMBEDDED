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

void main(void)
{
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
	// (see formula in Table 10-1)
	SYNC = 0; // asynchronous mode (TXSTA reg)
	SPEN = 1; // enable serial port (RCSTA reg)
	TX9 = 0; // 8-bit transmission (TXSTA reg)
	BRGH = 1; // asynchronous high-speed (TXSTA reg)
	CREN=1;	//enable continuous receive (RCSTA reg)
	TXEN = 1; // transmit enable (TXSTA reg)
	
	TRISB = 0x00; // set all ports in PORTB to output
	PORTB = 0x00; // all LEDs are off

	while(1) // foreground routine
	{
		while(!RCIF); // wait until receive buffer is full
		PORTB = RCREG;
	}
}
