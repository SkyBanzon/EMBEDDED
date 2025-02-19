#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit timerOverflowFlag = 0;
bit extInterruptFlag = 0;
bit updateFlag = 0;
int storedValue = 0x00;

void interrupt ISR()
{
	int inputData;
	GIE = 0;

	if(INTF == 1){
		INTF = 0;
		extInterruptFlag = 1;
		inputData = PORTD & 0x0F;

		if(inputData == 0x00){
			updateFlag = 1;
			storedValue = 0x01;
		}
		else if(inputData == 0x01){
			updateFlag = 1;
			storedValue = 0x02;
		}
		else if(inputData == 0x02){
			updateFlag = 1;
			storedValue = 0x03;
		}
		else if(inputData == 0x04){
			updateFlag = 1;
			storedValue = 0x04;
		}
		else if(inputData == 0x05){
			updateFlag = 1;
			storedValue = 0x05;
		}
		else if(inputData == 0x06){
			updateFlag = 1;
			storedValue = 0x06;
		}
		else if(inputData == 0x08){
			updateFlag = 1;
			storedValue = 0x07;
		}
		else if(inputData == 0x09){
			updateFlag = 1;
			storedValue = 0x08;
		}
		else if(inputData == 0x0A){
			updateFlag = 1;
			storedValue = 0x09;
		}
	}
	else if(TMR0IF == 1){
		TMR0IF = 0;
		timerOverflowFlag = 1;
	}

	GIE = 1;
}

void delay(int duration)
{
	int overflowCounter = 0;
    
    while (overflowCounter < duration)
    {
        if (timerOverflowFlag == 1)
        {
            overflowCounter++;
            timerOverflowFlag = 0;
        }
    }
}

void main()
{ 
	unsigned char count = 0x01;

	OPTION_REG = 0xC4;
	
	TRISB = 0xFF;
	TRISC = 0x00;
	TRISD = 0xFF;

	INTEDG = 1; // interrupt at rising edge
 	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag

	TMR0IF = 0;
    TMR0IE = 1;

	GIE = 1; // enables all unmasked interrupts

	PORTC = 0x00;

	while(1)
	{
		if(extInterruptFlag == 0)
		{
			while(extInterruptFlag != 1){ }
		}
		else if(count == 0x09)
        {
            count = 0x00;
        }
		else if(updateFlag == 1){
			count = storedValue;
			
		}
        else
        {
            count++;
        }
        updateFlag = 0;
        delay(50);
        PORTC = count;
	}
}
