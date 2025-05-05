#include <xc.h>

// CONFIG
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
float temp = 0;
int num1, num2;

void interrupt ISR() {
    GIE = 0;
    if (INTF) {
        INTF = 0;
        while (!TRMT);
        TXREG = num1 / 10; // Transmit tens digit
        while (!TRMT);
        TXREG = num1 % 10; // Transmit ones digit
    } else if (TMR0IF) {
        TMR0IF = 0;
        myTMR0IF = 1;
    }
    GIE = 1;
}

void delay(int count) {
    int of_count = 0;
    while (of_count < count) {
        if (myTMR0IF) {
            of_count++;
            myTMR0IF = 0;
        }
    }
}

int readADC() {
    int resultADC = 0;
    delay(30);
    GO = 1;
    while (GO_DONE == 1); // Wait for conversion complete
    resultADC = ADRESH;
    resultADC = (resultADC << 8) | ADRESL;
    return resultADC;
}

void instCtrl(unsigned char inst) {
    PORTD = inst;
    RB5 = 0; // RS=0
    RB7 = 1; // E=1
    RB6 = 0; // RW=0
    delay(1);
    RB7 = 0; // E=0
}

void dataCtrl(unsigned char data) {
    PORTD = data;
    RB5 = 1; // RS=1
    RB7 = 1; // E=1
    RB6 = 0; // RW=0
    delay(1);
    RB7 = 0; // E=0
}

void initLCD() {
    delay(30);
    instCtrl(0x38); // Function set: 8-bit, 2-line
    instCtrl(0x08); // Display OFF
    instCtrl(0x01); // Clear display
    instCtrl(0x06); // Entry mode set
    instCtrl(0x0C); // Display ON, Cursor OFF, Blink OFF
}

unsigned int convert(unsigned int data) {
    return data + 48; // Convert to ASCII
}

void main() {
    // UART Config
    SPBRG = 0x19; // 9600 baud @ 4MHz
    SYNC = 0;
    SPEN = 1;
    RX9 = 0;
    TX9 = 0;
    BRGH = 1;
    CREN = 1;
    TXEN = 1;

    TRISB = 0x01; // RB0 input for INT, rest output
    TRISD = 0x00;

    OPTION_REG = 0xC4;

    INTEDG = 1;
    INTF = 0;
    INTE = 1;

    TMR0IF = 0;
    TMR0IE = 1;

    GIE = 1;

    // ADC config
    ADCON1 = 0x80;
    ADCON0 = 0x01;

    initLCD();
    instCtrl(0x80);
    dataCtrl('T');
    dataCtrl('e');
    dataCtrl('m');
    dataCtrl('p');
    dataCtrl(':');

    int Dval = 0;
    while (1) {
        Dval = readADC();

        // Convert ADC value to temperature (LM35: 10mV/°C)
        temp = (float)Dval * (5.0 / 1024.0) * 100.0;

        num1 = (int)temp;                   // Integer part (e.g., 26)
        num2 = (int)((temp - num1) * 10);   // First decimal digit

        if (num2 > 9) {
            num2 = 0;
            num1 += 1;
        }

        // Display on LCD: tens, ones, decimal
        instCtrl(0x88);
        dataCtrl(convert(num1 / 10));  // Tens
        dataCtrl(convert(num1 % 10));  // Ones
        dataCtrl('.');
        dataCtrl(convert(num2));       // Tenths
        dataCtrl(0xDF);                // Degree symbol
        dataCtrl('C');
    }
}
