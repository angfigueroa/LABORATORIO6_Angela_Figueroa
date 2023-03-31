
// PIC16F887 Configuration Bit Settings
// Universidad del Valle de Guatemala
// Lab 6
// Angela Maria Figueroa Perez, 21395

// 'C' source line config statements

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
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#define _tmr0_value 0
#define __canal 
#define _XTAL_FREQ 8000000
#include <stdint.h>
#define __banderas ADCON0bits.CHS0

void __interrupt() isr (void){
       if (T0IF){
       
        PORTD++;
        TMR0 = _tmr0_value;
        T0IF = 0;
       }
    if (PIR1bits.ADIF == 1){
            if (ADCON0bits.CHS == 0b1000){
                //uint16_t adc = (ADRESH<<8) | ADRESL;
                PORTA = ADRESL;
            }
            else if (ADCON0bits.CHS == 0b1001){
               //uint16_t adc = (ADRESH<<8) | ADRESL;
                PORTC = ADRESL;
            }
          PIR1bits.ADIF  = 0; 
    }
        //PORTB = (char)(adc & 0b0011);
    
        
}
void setup(void);
void main(void){
    setup();
    //LOOP
    while(1){
       // TRISD ++;
        //__delay_ms(10);  
        
//        ADCON0bits.GO = 1;
//        __delay_ms(10);
       //_banderas = ~_banderas;
        if (ADCON0bits.GO == 0){
            
            if ( ADCON0bits.CHS == 0b1000)
                ADCON0bits.CHS = 0b1001;
            else if (ADCON0bits.CHS == 0b1001)
                ADCON0bits.CHS = 0b1000;  
            
           __delay_ms(10);
            ADCON0bits.GO = 1;
        }
       
    
       
        
        //uint16_t adc = (ADRESH<<8) | ADRESL;
       
    }
    
    return;
}
void setup(void){
    ANSEL = 0;
    //ANSELH = 0;
    TRISC = 0;
    TRISA = 0;
    TRISD = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.SCS = 1;
    
    OPTION_REGbits.T0CS =0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b100;
    TMR0 = _tmr0_value;
    
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    INTCONbits.GIE = 1;
    
    
    //configuración del ADC
    TRISB = 0b11111111;
    ANSELH = 0b00001100;
    
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0b1000;
    __delay_ms(1);
    
    ADCON1bits.ADFM = 1;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADON = 1;
    ADIF = 0;
}
