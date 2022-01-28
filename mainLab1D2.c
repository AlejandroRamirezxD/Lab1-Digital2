/*
  Archivo:  LAB1.c
  Autor:    Alejandro Ramirez Morales
  Creado:   24/enero/22
 */

// PIC16F887 Configuration Bit Settings

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
#include <stdint.h>
#include "ADC.h"
#include "HEXDISP.h"

#define _XTAL_FREQ 4000000
/*
 +----------------------------------------------------------------------------+
 |                             VARIABLES GLOBALES                             |
 +----------------------------------------------------------------------------+
 */
uint8_t estadoRB1 = 0;
uint8_t estadoRB2 = 0;
uint8_t turno;

uint8_t turnoIPORTB = 0;
uint8_t cuenta = 0;

int cuenta2;

uint8_t unidades;
uint8_t decenas;

uint8_t display0;
uint8_t display1;
/*
 +----------------------------------------------------------------------------+
 |                          PROTOTIPOS DE FUNCIONES                           |
 +----------------------------------------------------------------------------+
 */
void setup(void);
void resetTMR1(void);
void ValoresDisplay(void);
void Displayturno (void);
void ProteccionBotones (void);
void CambioCanalADC (void);
/*
 +----------------------------------------------------------------------------+
 |                               INTERRUPCIONES                               |
 +----------------------------------------------------------------------------+
 */
void __interrupt() isr (void)
{
    // INTERRUPCION DEL TIMER 1
    if(PIR1bits.TMR1IF){
        // Reset del tmr1 al suceder un segundo
        //PORTC++;
        resetTMR1();    
        
        // Multiplex
        switch (turno){
            case 0:
                PORTC = display0;
                PORTDbits.RD0 = 1;
                PORTDbits.RD1 = 0;
                break;
            case 1:
                PORTC = display1;
                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 1;
                break;
            default:
                turno = 0;
                break;
        }
    }
    
    // INTERRUPCION DEL ADC
    if (PIR1bits.ADIF){
        if (ADCON0bits.CHS ){
           cuenta = ADRESH;
           //PORTD = cuenta;
        }
        PIR1bits.ADIF = 0;
    }
    
    
    // INTERRUPCION DEL PORTB   
    if(INTCONbits.RBIF){
        // El turno set indica que la interrupcion esta sucediendo
        turnoIPORTB = 1;
        
        // El botón ejecuta su función sin estar previamente presionado
        if(!PORTBbits.RB0 == 1 && estadoRB1 == 1){
            PORTA++;
        }
                            
        // El botón ejecuta su función sin estar previamente presionado
        if(!PORTBbits.RB1 == 1 && estadoRB2 == 1){
            PORTA--;
        }
        
        INTCONbits.RBIF = 0;    // Limpiar banderta de PORTB
        turnoIPORTB = 0;              // Termina interrupcion del PORTB   
    }
}

/*
 +----------------------------------------------------------------------------+
 |                                   LOOP                                     |
 +----------------------------------------------------------------------------+
 */
void main(void) 
{
    setup(); // Se ejecuta funcion setup
    ADCON0bits.GO = 1; // El ciclo A/D esta en progreso
    while(1)
    {
        ValoresDisplay(); // Genera los valores de cada digito del diplay
        Displayturno();   // Dicta el turno del display
        ProteccionBotones();
        CambioCanalADC();
        
        if(PORTA <= cuenta){
            PORTEbits.RE0 = 1;
        }
        else{
            PORTEbits.RE0 = 0;
        }
    }
}

/*
 +----------------------------------------------------------------------------+
 |                            SETUP Y FUNCIONES                               |
 +----------------------------------------------------------------------------+
 */

void resetTMR1 (void){
    TMR1H = 254;             // preset for timer1 MSB register
    TMR1L = 12;             // preset for timer1 LSB register
    PIR1bits.TMR1IF = 0;        // Bandera TMR1 apagada
}

void ProteccionBotones (void){
    // Banderas para el RB1 (Jugador 1)
        if(PORTBbits.RB0 == 0 && turnoIPORTB == 0){
            estadoRB1 = 0; // No afecta al conteo
        }
        else if (PORTBbits.RB0 == 1 && turnoIPORTB == 0){
            estadoRB1 = 1; // Si afecta al conteo
        }
        
        // Bandera para el RB2 (Jugador 2)
        if(PORTBbits.RB1 == 0 && turnoIPORTB == 0){
            estadoRB2 = 0; // No afecta al conteo
        }
        else if (PORTBbits.RB1 == 1 && turnoIPORTB == 0){
            estadoRB2 = 1; // No afecta al conteo
        } 
        return;
}

void ValoresDisplay (void){
    // unidades es el modulo entres cuenta y 16
        unidades = cuenta%16;
    // cociente0 son los primeros dos digitos de la division entre cuenta y 16
        decenas = cuenta/16;

   // Pasar números al display
        display1 = ArregloHex(unidades);
        display0 = ArregloHex(decenas);
        return;
}

void Displayturno (void)
{
    // Actualiza el valor de turno para realizar multiplexeo 
    switch(turno){
        case 0:
            turno = 1 ;
            break;  
        case 1:
            turno = 0 ;
            break; 
        default:
            turno = 0 ;
            break;   
    }  
    return;
}

void CambioCanalADC (void){
    // Posible cambio de canal
    if (ADCON0bits.GO == 0){
        ADCON0bits.CHS = 13;
        __delay_us(50);         // pausa para cambio de canal
        ADCON0bits.GO = 1;
    }
    return; 
}

void setup(void)
{
    // Ports 
    ANSEL   =   0;              // Digital Ports
    ANSELH  =   0b00100000;
    
    TRISA   =   0;              // PORTA - salida
    TRISB   = 0b100011;
    TRISC   =   0;              // PORTC - salida
    TRISD   =   0;              // PORTD - salida
    TRISE   =   0;              // PORTE - salida
    
    PORTA   =   0;              // PORTA en 0
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    PORTC   =   0b01000000; // Display en 0
    PORTD   =   0;              // PORTD en 0
    PORTE   =   0;              // PORTB en 0
    
    // Botones
    OPTION_REGbits.nRBPU = 0;   // Habilitar pull ups
    WPUBbits.WPUB0 = 1;         // RB0 con pull up
    WPUBbits.WPUB1 = 1;         // RB1 con pull up
    
    // IOCB
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
        
    // Reloj
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Activar reloj interno
    
    // Interrupciones
    INTCONbits.GIE  = 1;        // Interrupciones globales activadas
    INTCONbits.PEIE = 1;        // Interrupciones perifericas activadas
    INTCONbits.RBIE = 1;        // Interrupion del puerto b activada
    INTCONbits.RBIF = 0;        // Bandera interrupcion portb apagada
    PIE1bits.TMR1IE = 1;        // Interrupcion del TMR1
    PIR1bits.TMR1IF = 0;        // Bandera TMR1 apagada    
    PIR1bits.ADIF = 0;      // A/D conversion no ha empezado o completado

    // Timer1 Registers Prescaler= 1 - TMR1 Preset = 65036 - Freq = 500.00 Hz - Period = 0.002000 seconds
    T1CONbits.T1CKPS1 = 0;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 0;   // bit 4
    T1CONbits.T1OSCEN = 0;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
        // Reset del tmr1
    resetTMR1(); 
    
    // Librería ADC
    ADC(0,13);
    __delay_us(50);
    return;   
}