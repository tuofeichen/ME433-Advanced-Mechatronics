#include<xc.h>           // processor SFR definitions

#ifndef _SPI_H    /* Guard against multiple inclusion */
#define _SPI_H

#define CS  LATBbits.LATB15
// MISO: RB8 // tentative 
// MOSI: RB13
// SCLK: RB14
// CS:   RB15

unsigned char spi_io(unsigned char o);
void spi_init();

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
