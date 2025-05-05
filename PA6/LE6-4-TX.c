#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

void interrupt ISR()
{
	GIE = 0;
	if(ADIF){
	
	}

	GIE = 1;

}



void main()
{	
	SPBRG = 0x19;       // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
	                    // (see formula in Table 10-1)
	SYNC = 0;           // asynchronous mode (TXSTA reg)
	SPEN = 1;           // enable serial port (RCSTA reg)
	TX9 = 0;            // 8-bit transmission (TXSTA reg)
	BRGH = 1;           // asynchronous high-speed (TXSTA reg)
	TXEN = 1;           // transmit enable (TXSTA reg) 
	TRISC6 = 0;
	TRISC7 = 1;
	unsigned char sendNum;
	
	TRISD = 0xFF;
	unsigned char DATA;
		
	while(1)
	{	

		
			while(RD4){
				DATA = PORTD & 0x0F;
				switch(DATA){
					case 0x00:
						sendNum = 0x01;
						break;
					case 0x01:
						sendNum = 0x02;
						break;
					case 0x02:
						sendNum = 0x03;
						break;
					case 0x04:
						sendNum = 0x04;
						break;
					case 0x05:
						sendNum = 0x05;
						break;
					case 0x06:
						sendNum = 0x06;
						break;
					case 0x08:
						sendNum = 0x07;
						break;
					case 0x09:
						sendNum = 0x08;
						break;
					case 0x0A:
						sendNum = 0x09;
						break;
					case 0x0D:
						sendNum = 0x00;
						break;
				}
				while(RD4);
			}
			
			while(!TRMT);
			TXREG = sendNum;
		
		
		
		
	}	
		
}