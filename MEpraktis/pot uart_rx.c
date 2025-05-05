
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


int whole = 0;
int decimal = 0;
bit control = 0;

// Simple delay
void delay(int cnt) {
    while(cnt--)
}



void main(void) {
    SPBRG = 0x19;  // 9600 baud @ 4MHz
    SYNC = 0;      // Asynchronous
    SPEN = 1;      // Enable serial port
    RX9 = 0;       // 8-bit reception
    BRGH = 1;      // High-speed
    CREN = 1;      // Enable continuous receive
    TXEN = 1;      // Enable transmission (if needed)
  
    TRISB = 0x00;  // Decoder connected to PORTB
    TRISC = 0x80;  // RC0, RC1, RC2 as output for display select
  
    RC0 = 0;
    RC1 = 0;
    RC2 = 0;

    while(1) {
        // === Receive new data via UART ===
        while(!RCIF);            // Wait for 1st char (whole)
        whole = RCREG & 0x0F;    // Mask to 4 bits (0–9)

        while(!RCIF);            // Wait for 2nd char (decimal)
        decimal = RCREG & 0x0F;

        // isplay loop
        while(1) {
            if (RCIF) break;     // Exit if new UART data arrives

            if (control) {
                // Show whole number
                RC0 = 0;
                RC1 = 1;
                RC2 = 1;
                PORTB = whole & 0x0F;
            } else {
                // Show decimal digit
                RC0 = 1;
                RC1 = 0;
                RC2 = 0;
                PORTB = decimal & 0x0F;
            }

            control = !control;
            delay(500);
        }
    }
}
