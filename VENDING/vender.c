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

const unsigned char menu1[] = "[1] Coke Medium";
const unsigned char menu2[] = "[2] Coke Large";
const unsigned char menu3[] = "[3] Sprite Medium";
const unsigned char menu4[] = "[4] Sprite Large";
const unsigned char dispensing[] = "Dispensing...";
const unsigned char stop[] = "Emergency Stop...";

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
void instCtrl (unsigned char INST)
{
	PORTC = INST; // load instruction to PORTC
	RB4 = 0; // set RS to 0 (instruction reg)
	RB5 = 1; // set E to 1
	RB6 = 0; // set RW to 0 (write)
	delay(1); // 1 ms delay (estimate)
	RB5 = 0; // set E to 0 (strobe)
}

void initLCD()
{
	delay(1);        // Small delay to ensure LCD is ready before initialization
	instCtrl(0x38);  // Set LCD to 8-bit mode, 2-line display, and 5x8 font
	instCtrl(0x08);  // Display OFF, cursor OFF, blink OFF
	instCtrl(0x01);  // Clear display and reset cursor to home position
	instCtrl(0x06);  // Set entry mode: cursor moves right, display shift OFF
	instCtrl(0x0F);  // Display ON, cursor ON, cursor blinking
}

void dataCtrl (unsigned char DATA)
{
	PORTC = DATA; // load data to PORTC
	RB4 = 1; // set RS to 1 (data reg)
	RB5 = 1; // set E to 1
	RB6 = 0; // set RW to 0 (write)
	delay(1); // 1 ms delay (estimate)
	RB5 = 0; // set E to 0 (strobe)
}

void printString(const unsigned char *lines){
	int i;
	for(i=0;lines[i]!='\0';i++){
		dataCtrl(lines[i]);
	} 
}

void dispMenu(){
	instCtrl(0x01);		//clear screen
	instCtrl(0x80);
	printString(menu1);
	instCtrl(0xC0);
	printString(menu2);
	instCtrl(0x94);
	printString(menu3);
	instCtrl(0xD4);
	printString(menu4);
}

void dispEmergency(){
	instCtrl(0x01);		//clear screen
	instCtrl(0xC2);
	printString(stop);
	
	for(int i = 5;i>=1;i--){
		instCtrl(0x9A);
		dataCtrl(i+0x30);
		dataCtrl('s');
		delay(122);
	}
	myINTF=0;
}

void dispDispensingReg(){
	instCtrl(0x01);		//clear screen
	instCtrl(0xC3);
	printString(dispensing);
	
	for(int i = 3;i>=1;i--){
		instCtrl(0x9A);
		dataCtrl(i+0x30);
		dataCtrl('s');
		delay(122);
		
		if(myINTF){
			i=0;
			dispEmergency();
		}
		
	}

}

void dispDispensingLrg(){
	instCtrl(0x01);		//clear screen
	instCtrl(0xC3);
	printString(dispensing);
	
	for(int i = 5;i>=1;i--){
		instCtrl(0x9A);
		dataCtrl(i+0x30);
		dataCtrl('s');
		delay(122);
		
		if(myINTF){
			i=0;
			dispEmergency();
		}
		
	}

}

void main(){
	
	TRISB = 0X0F;	//intr button and LCD initializatoin
	TRISC = 0X00;	//LCD data
	TRISD = 0XFF;	//keypad
	OPTION_REG = 0XC4;
	
	INTEDG = 1;// RISING EDGE MMDMFMDFMHFGHMFGH
	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag
 	
	TMR0IE = 1;	//timer0 intr enable	dapat naay tmr0if sa imo ISR para mo gana
	TMR0IF = 0;	//timer0 intr flag =0

 	GIE = 1; // enables all unmasked interrupt

 	///////////////////////////////////////////////////////////

 	initLCD();
 	
	unsigned char num;
	
	while(1){
		dispMenu();
		while(!RD4){}
		while(RD4){
			num = PORTD & 0x0F;
			
			switch(num){
				case 0x00:dispDispensingReg();break;
				case 0x01:dispDispensingLrg();break;
			}
			
		}
	}
}
