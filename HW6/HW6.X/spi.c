#include <proc/p32mx250f128b.h>

#include "spi.h"


//SPI io with SPI channel 1
unsigned char spi_io(unsigned char o)
{
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF)
    {
        ; //        receiving bytes
    }
    return SPI1BUF; 

}

//setup pins for SPI functions
void spi_init()
{
    
    TRISBbits.TRISB15 = 0; //set B15 to be output  CS
    TRISBbits.TRISB14 = 0; //set B14 to be output  SCLK
    TRISBbits.TRISB13 = 0; //set B13 to be output  MOSI
    TRISBbits.TRISB8 = 1;  //set B8 to be input    MISO
   
//    setup pin functions for MISO and MOSI
    RPB13Rbits.RPB13R = 0b0011; // MOSI
    SDI1Rbits.SDI1R = 0b0100;   // MISO
    
    CS = 1;// set chip select high
    
//    setup SPI registers
    SPI1CON = 0; // reset SPI register
    SPI1BUF;
    SPI1BRG = 0x03; //baud rate to 2 MHz [SPI4BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0;
    SPI1CONbits.CKE = 1; // data read on falling edge
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.ON    = 1; // turn on 
    
}


