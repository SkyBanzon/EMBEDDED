#include <xc.h>
#include <stdio.h>
#include <math.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF, CPD = OFF, WRT = OFF, CP = OFF

#define _XTAL_FREQ 4000000

void USART_tx(char ch) {	//lets TXREG hold character to be transmitted
    while (!TXIF);
    TXREG = ch;
}

void USART_send_string(const char* str) {
    while (*str) USART_tx(*str++);
}


unsigned int ADC_read() {
    GO = 1;
    while (GO);
    return (ADRESH << 8) | ADRESL;
}

void delay(int cnt) {
    while(cnt--){}
}

void main(void) {
    TRISA = 0x01;  // RA0 as input (LM35)
    TRISB = 0x00;
    
    TXEN = 1; BRGH = 1; SYNC = 0; SPEN = 1; TX9=0;
    SPBRG = 0x19; // ~9600 baud @ 4MHz
    
    ADCON1 = 0x80; // Right justify, Vref=Vdd, AN0 analog
    ADCON0 = 0x41; // Channel 0, ADC ON
    
    delay(30000);
    
    char buffer[32];
    unsigned int adc_val;
    float temp;

    while (1) {
        adc_val = ADC_read();
        temp = adc_val * 0.488; // Convert to °C
        
        sprintf(buffer, "Temp: %.2f C\r\n", temp);
        USART_send_string(buffer);
        
        __delay_ms(1000);
    }
}
