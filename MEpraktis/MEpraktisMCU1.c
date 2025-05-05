
#include <xc.h>

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
float temp =0;
int num1, num2;


void interrupt ISR (){
	GIE = 0;
	if(INTF){
		INTF=0;
		while (!TRMT);
		TXREG = num1;

		while (!TRMT);
		TXREG = num2;
	}else if(TMR0IF){
		TMR0IF = 0;
		myTMR0IF = 1;
	}
	GIE = 1;
}


void delay(int count){
	int of_count = 0;
	while (of_count < count){
		if (myTMR0IF){
			of_count++;
			myTMR0IF = 0;
		}
	}
}


int readADC(){
	int resultADC =0;
	delay(30);
	GO=1;
	while(GO_DONE=1);
	
	resultADC=ADRESH;
	resultADC= resultADC<<8;
	resultADC= resultADC|ADRESL;
	return resultADC;
}


void instCtrl(unsigned char inst)
{
	PORTD = inst;
	RB5 = 0;	//RS=0
	RB7 = 1;	//E=1
	RB6 = 0;	//RW=0
	delay(1);
	RB7 = 0;	//E=1
}

void dataCtrl(unsigned char data)
{
	PORTD = data;
	RB5 = 1;	//RS=1
	RB7 = 1;	//E=1
	RB6 = 0;	//RW=0
	delay(1);
	RB7 = 0;	//E=0
}

void initLCD()
{
	delay(30);
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0C); // display on; cursor on; blink off
}


unsigned int convert(unsigned int data){
	data += 48;		//convert value to ASCII for displaying
	return data;
}


void main(){
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
   	// (see formula in Table 10-1)
   	SYNC = 0; 	// asynchronous mode (TXSTA reg)
   	SPEN = 1; 	// enable serial port (RCSTA reg)
   	RX9 = 0; 	// 8-bit reception (TXSTA reg)
	TX9 = 0;
   	BRGH = 1; 	// asynchronous high-speed (TXSTA reg)
   	CREN = 1; 	// enable continuous receive (RCSTA reg)
   	TXEN = 1; 	// transmit enable (TXSTA reg)
	
	TRISB=0x01;
	TRISD=0x00;

   	OPTION_REG = 0xC4;
	
	INTEDG = 1;
	INTF = 0;
	INTE = 1;

	TMR0IF = 0;
	TMR0IE = 1;

	GIE = 1;

	ADCON1 = 0x80; 	// result: right Justified, clock: FOSC/2
					// all ports in PORTA are analog
					// VREF+: VDD, VREF-: VSS
	ADCON0 = 0x01; 	// clock: FOSC/2, analog channel: AN0,
					// A/D conversion: STOP, A/D module: ON

	initLCD();
	instCtrl(0x80);
	//display "Voltage:"
	dataCtrl('V');
	dataCtrl('o');
	dataCtrl('l');
	dataCtrl('t');
	dataCtrl('a');
	dataCtrl('g');
	dataCtrl('e');
	dataCtrl(':');
	
	int Dval=0;
	while(1){
		Dval=readADC();
		
		temp=(float)Dval*(4.9/100);
		num1=(int)temp/10;
		num2=(int)temp-(num1*10);
		
		if(num2>9){
			num2=0;
			num1 +=1;
		}
		instCtrl(0x88);
		dataCtrl(convert(num1));
		dataCtrl('.');
		dataCtrl(convert(num2));
	}
	
}
