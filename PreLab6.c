/* 
 * File:   PRELAB_6.c
 * Author: ANGELA Maria Figueroa Perez, 21395
 *
 * Created on 26 de marzo de 2023, 14:57
 */

// PIC16F887 Configuration Bit Settings


// 'C' source line config statements


// CONFIG1
#pragma config FOSC     =   INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE     =   OFF             // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    =   OFF             // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    =   OFF             // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP       =   OFF             // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      =   OFF             // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    =   OFF             // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO     =   OFF             // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN    =   OFF             // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP      =   OFF             // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
 
// CONFIG2
#pragma config BOR4V    =   BOR40V          // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      =   OFF             // Flash Program Memory Self Write Enable bits (Write protection off)
// DEFINIMOS LAS VARIABLES
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL
//INTERRUPCIONES
void __interrupt() isr(void) 
{ 
    if (T0IF) 
    {
        PORTAbits.RA0 = ~PORTAbits.RA0; 
        TMR0 = 56; 
        T0IF = 0; 
    }
}
//Retardos en milisegundos
void delay_ms(uint16_t ms) 
{
    for (uint16_t i = 0; i < ms; i++)
    {
        __delay_ms(1); 
    }
}
//Config Registros en otras palabras los pines
void setup(void) 
{
    ANSEL  = 0; // entradas/salidas digitales
    ANSELH = 0;
    TRISA0 = 0; //  (RA0) salida digital
    
    // Configurando el oscilador 
    OSCCONbits.IRCF = 0b111; // Configura el oscilador interno a 8MHz
    OSCCONbits.SCS = 1; 
    
    // Config del el timer0
    OPTION_REGbits.T0CS = 0; 
    OPTION_REGbits.PSA = 0; 
    OPTION_REGbits.PS = 0b111; // Timer0 a 1:256
    TMR0 = 56; // INTERRUPCION DE 200 US
    
    // Configurando la interrupción del TMR0
    INTCONbits.T0IF = 0; 
    INTCONbits.T0IE = 1; 
    INTCONbits.GIE = 1; 
}

void main(void) 
{ 
    setup(); 
    
    while(1) 
    {
        PORTA++; 
        delay_ms(165); // se genera  165 ms
    }
    
    return;
}
