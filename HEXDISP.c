#include "HEXDISP.h"

/*
 +----------------------------------------------------------------------------+
 | La función te permite agregar un valor al FOSC y ELEGIR UN ANSEL.          |
 +----------------------------------------------------------------------------+
 */
// No olvidar apagar la bandera y agregar el delay


uint8_t ArregloHex (uint8_t numero){
    switch(numero){
        case 0:
            return 0b11101011;
            break;    
        case 1:
            return 0b10001000;
            break;    
        case 2:
            return 0b10110011;
            break;
        case 3:
            return 0b10111010;
            break; 
        case 4:
            return 0b11011000;
            break;
        case 5:
            return 0b01111010;
            break;
        case 6:
            return 0b01111011;
            break;
        case 7:
            return 0b10101000;
            break;
        case 8:
            return 0b11111011;
            break;
        case 9:
            return 0b11111000;
            break;
        case 10: // A
            return 0b11111001;
            break;
        case 11: // B b
            return 0b01011011;
            break;
        case 12: // C
            return 0b01100011;
            break;
        case 13: // d
            return 0b10011011;
            break;
        case 14: // E
            return 0b01110011;
            break;
        case 15: // F
            return 0b01110001;
            break;
            
        default:
            return 0b11101011;
            break;   
    }   
}
/*
uint8_t ArregloHex (uint8_t numero){
    switch(numero){
        case 0:
            return 0b00111111;
            break;    
        case 1:
            return 0b00000110;
            break;    
        case 2:
            return 0b01011011;
            break;
        case 3:
            return 0b01001111;
            break; 
        case 4:
            return 0b01100110;
            break;
        case 5:
            return 0b01101101;
            break;
        case 6:
            return 0b01111101;
            break;
        case 7:
            return 0b00000111;
            break;
        case 8:
            return 0b01111111;
            break;
        case 9:
            return 0b01101111;
            break;
        case 10: // A
            return 0b01110111;
            break;
        case 11: // B
            return 0b01111100;
            break;
        case 12: // C
            return 0b00111001;
            break;
        case 13: // d
            return 0b01011110;
            break;
        case 14: // E
            return 0b01111001;
            break;
        case 15: // F
            return 0b01110001;
            break;
            
        default:
            return 0b00111111;
            break;   
    }   
}

*/