#include "ADC.h"

/*
 +----------------------------------------------------------------------------+
 | La función te permite agregar un valor al FOSC y ELEGIR UN ANSEL.          |
 +----------------------------------------------------------------------------+
 */
// No olvidar apagar la bandera y agregar el delay

void ADC (uint8_t FOSC, uint8_t ansel){
    PIE1bits.ADIE = 1;      // Activa la interrupción de ADC
    // Configuración del ADC
    ADCON1bits.ADFM  = 0;   // Justificado a la izquierda
    ADCON1bits.VCFG1 = 0;   // Referencia como tierra
    ADCON1bits.VCFG0 = 0;   // Referencia poder
    
    ADCON0bits.ADCS = FOSC; // Fosc/2 POR EL RELOJ A 1MHz
    ADCON0bits.CHS  = ansel;    // Ansel 13
    ADCON0bits.ADON = 1;    // ADC activo
    //__delay_us(50);
}

