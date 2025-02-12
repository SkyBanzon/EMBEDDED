#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

const unsigned char line_addresses[4] = {0x80, 0xC0, 0x94, 0xD4};

unsigned char line = 0, pos = 0;  // Track current line and position

void delay (unsigned char MUL)
{
	int i,j;
	for (i=MUL; i != 0; i--) // loop until i = 0
	{
		for (j=0; j < 256; j++); // loop until j = 0
		{
		}
	}
}

void instCtrl (unsigned char INST)
{
	PORTB = INST; // load instruction to PORTB
	RC0 = 0; // set RS to 0 (instruction reg)
	RC1 = 1; // set E to 1
	RC2 = 0; // set RW to 0 (write)
	delay(1); // 1 ms delay (estimate)
	RC1 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	if (line >= 4) { 	// If all 4 lines are filled, clear display
        instCtrl(0x01); // Clear display
        delay(2);  
        line = 0; 
        pos = 0;
        instCtrl(line_addresses[line]); // Reset to first line
    }
	else if (pos >= 20) { 
        line++;
        instCtrl(line_addresses[line]);
        pos = 0;
    }

	PORTB = DATA; // load data to PORTB
	RC0 = 1; // set RS to 1 (data reg)
	RC1 = 1; // set E to 1
	RC2 = 0; // set RW to 0 (write)
	delay(1); // 1 ms delay (estimate)
	RC1 = 0; // set E to 0 (strobe)

	pos++;
}


void initLCD()
{
	delay(1);
	instCtrl(0x38);
	instCtrl(0x08);
	instCtrl(0x01);
	instCtrl(0x06);
	instCtrl(0x0F);
}

unsigned char readKeypad() {
    unsigned char key = 0xFF; // Default no key pressed

    if (RD4) { // A key is being pressed
        delay(20); // Debounce delay
        key = PORTD & 0x0F; // Read key value
        while (RD4); // Wait until key is released
    }

    return key;
}

void main(){
	unsigned char key;
	OPTION_REG = 0xC0; // configuring the OPTION register
	TRISA = 0x00; // set all of PORTB as output    LEDs
	TRISC = 0x00; // set ALL PORT C TO OUTPUT	   LCD INIT
	TRISB = 0x00; // set all of PORTB as output    LCD DISPLAY
	TRISD = 0X1F; //SET PORTD AS INPUT              KEYPAD INPUT
	
	initLCD();

	while(1){
		key=readKeypad();

		switch(key){
        	case 0x00: dataCtrl('1'); break;
            case 0x01: dataCtrl('2'); break;
            case 0x02: dataCtrl('3'); break;
            case 0x04: dataCtrl('4'); break;
            case 0x05: dataCtrl('5'); break;
            case 0x06: dataCtrl('6'); break;
            case 0x08: dataCtrl('7'); break;
            case 0x09: dataCtrl('8'); break;
            case 0x0A: dataCtrl('9'); break;
            case 0x0C: dataCtrl('*'); break;
            case 0x0D: dataCtrl('0'); break;
            case 0x0E: dataCtrl('#'); break;
			default: break;
		}

	}
}


