#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit TMR0IF_flag = 0;
bit INTF_flag = 0;

unsigned char MSBnumber = 0x00;
unsigned char LSBnumber = 0x00;

unsigned char keypress = ' ';

#define second 122
int speed = 122;
bit countDirection = 0;

void interrupt ISR(){
	GIE = 0;
	if(INTF == 1){
		INTF = 0;	
		INTF_flag ^= 1;
	}
	else if (TMR0IF == 1){
		TMR0IF = 0;
		TMR0IF_flag ^= 1;
	}
	
	GIE = 1;
}

void delay(int count)
{
	int OF_count = 0;
    
    while (OF_count < count)
    {
        if (TMR0IF_flag == 1)
        {
            OF_count++;
            TMR0IF_flag = 0;
        }
    }
}

void delayLCD (int MUL)
{
	int i,j;
	for (i=MUL; i != 0; i--){
		for (j=0; j < 256; j++){} // loop until j = 0
	} // loop until i = 0
	
}

void instCtrl (unsigned char INST)
{
	PORTC = INST; // load instruction to PORTB
	RB1 = 0; // set RS to 0 (instruction reg)
	RB2 = 0; // set RW to 0 (write)
	RB3 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RB3 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTC = DATA; 		// load data to PORTB
	RB1 = 1; 			// set RS to 1 (data reg)
	RB2 = 0; 			// set RW to 0 (write)
	RB3 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RB3 = 0; 			// set E to 0 (strobe)
}

void initLCD()
{
	delayLCD(1); // LCD startup about approx 15ms
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0C); // display on; cursor on; blink off	
}


void numberDisplay(unsigned char MSB, unsigned char LSB){
	instCtrl(0x9D);
	dataCtrl(MSB + 0x30);
	dataCtrl(LSB + 0x30);
}


void main(){
	OPTION_REG = 0xC4;
	TRISB = 0x01;
	TRISC = 0x00;
	TRISD = 0xFF;

	INTEDG = 1; // interrupt at rising edge
 	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag

	TMR0IF = 0;
    TMR0IE = 1;

	GIE = 1; // enables all unmasked interrupt
	

	initLCD();
	numberDisplay(MSBnumber,LSBnumber);
	
	while(1){
		if(INTF_flag == 0){
			switch(PORTD & 0x0F){
				case 0x0C: 
					countDirection = 1;
					break;
				case 0x0E: 
					countDirection = 0;
					break;
				default:
					break;
			}
			if(countDirection == 1){					//COUNT UP
				if(LSBnumber == 0x09){
					LSBnumber = 0x00;
					
					if(MSBnumber == 0x02 && LSBnumber == 0x04){
						MSBnumber = 0x00;
						LSBnumber = 0x00;
					}
					else{
						MSBnumber++;
					}				
				}
				else{
					LSBnumber++;
				}
			}
			
			else if(countDirection == 0){			//COUNT DOWN
				if(LSBnumber == 0x00){
					if(MSBnumber == 0x00){
						MSBnumber = 0x02;
						LSBnumber = 0x04;
					}
					else{
						MSBnumber--;
						LSBnumber = 0x09;
					}				
				}
				else{
					LSBnumber--;
				}
			}
			delay(second);
			numberDisplay(MSBnumber,LSBnumber);
		}
		if(INTF_flag == 1){
			numberDisplay(MSBnumber,LSBnumber);
		}
		
	}

}