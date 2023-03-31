/*
 * File:   POSTLab_6.c
 * Author: Angela Figueroa 
 *
 * Created on 28 de marzo de 2023, 02:00 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
       
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#define _tmr0_value 237

/*------------------------------------------------------------------------------
 * Displays
 ------------------------------------------------------------------------------*/

unsigned char display[] =
{
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
};


/*------------------------------------------------------------------------------
 * VARIABLES
 ------------------------------------------------------------------------------*/

unsigned int centenas, unidades, decenas, residuo;
unsigned int numero = 0;

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES
 ------------------------------------------------------------------------------*/
void setup(void);

/*------------------------------------------------------------------------------
 * INTERRUPCIONES
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if (T0IF == 1){
PORTD++;
    }
   //Nuestras operaciones
    if(T0IF == 1){
        numero = numero*1.96;
        centenas = numero/100;
        residuo = numero%100;
        decenas = residuo/10;
        unidades = residuo%10;
        T0IF = 0;
    }
   
    if(PIR1bits.ADIF){              // Fue interrupci?n del ADC?
        if(ADCON0bits.CHS == 12){    // Verificamos sea AN12 el canal seleccionado
            PORTA = ADRESH;         // Mostramos ADRESH en PORTC
        }
       
        else
            numero = ADRESH;
            PIR1bits.ADIF = 0;          // Limpiamos bandera de interrupci?n
    }
}

/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
   
    __delay_us(50);
    ADCON0bits.GO = 1;
   
    while(1){
       
        PORTC = display[centenas];
        PORTE = 0B001;
        __delay_ms(10);
        PORTE = 0B000;
       
        PORTC = display[decenas];
        PORTE = 0B010;
        __delay_ms(10);
        PORTE = 0B000;
       
        PORTC = display[unidades];
        PORTE = 0B100;
        __delay_ms(10);
        PORTE = 0B000;
       
       
        if(ADCON0bits.GO == 0){                
            if(ADCON0bits.CHS == 12) {  
                ADCON0bits.CHS = 10;    // Cambio de canal
            }
            else
                ADCON0bits.CHS = 12;    // Cambio de canal
                __delay_us(100);                 // Tiempo de adquisicion
                ADCON0bits.GO = 1;              // Iniciamos proceso de conversion
        }
    }
}

/*------------------------------------------------------------------------------
 * CONFIGURACION
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0b00000011;        
   
    TRISA = 0;
    PORTA = 0;
   
    PORTE = 0;
    TRISE = 0;
   
    TRISB = 0b00000011;
    PORTB = 0b00000011;
           
    TRISC = 0;
    PORTC = 0;
   
    TRISD = 0XFE;
    PORTD = 0;
   
    // CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0; // 4MHZ
    OSCCONbits.SCS = 1;  // OSCILADOR INTERNO
   
    // CONFIGURACION DEL TMR0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;     // HABILITAMOS EL PRESCALER
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1; // PRESCALER 1:256
   
    // CONFIGURACION ADC
    ADCON0bits.ADCS = 0b01;     // Fosc/8
    ADCON1bits.VCFG0 = 0;       // VDD
    ADCON1bits.VCFG1 = 0;       // VSS
    ADCON0bits.CHS = 12;    // Seleccionamos el AN12
   
   
    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
    __delay_us(50);             // Sample time
    ADCON0bits.ADON = 1;        // Habilitamos modulo ADC
   
    // CONFIGURACION INTERRUPCIONES
    PIR1bits.ADIF = 0;          // Limpiamos bandera de ADC
    PIE1bits.ADIE = 1;          // Habilitamos interrupcion de ADC
    INTCONbits.PEIE = 1;        // Habilitamos int. de perifericos
    INTCONbits.GIE = 1;         // Habilitamos int. globales        
    INTCONbits.T0IE = 1;        
    INTCONbits.T0IF = 0;        
    INTCONbits.TMR0IF = 0;        
    INTCONbits.TMR0IE = 1;        
    INTCONbits.RBIF = 0;        
   
    return;
}
