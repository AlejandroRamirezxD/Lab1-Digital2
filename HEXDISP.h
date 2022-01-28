
/*
 +----------------------------------------------------------------------------+
 | La función te permite agregar un valor al FOSC y ELEGIR UN ANSEL.          |
 +----------------------------------------------------------------------------+
 */
// No olvidar apagar la bandera y agregar el delay

#ifndef HEXDISP_H
#define	HEXDISP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

uint8_t ArregloHex (uint8_t numero);

#endif	/* HEXDISP_H */

