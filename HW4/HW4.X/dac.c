#include "dac.h"
#include "spi.h"

#define PI 3.1415926

void generateSinArray(int res, char* out)
{
    int i = 0;
            
    for (i = 0; i < res; i++)
    {
        
        *out++= 127 + 127*sin(2*PI*i/res);//
    }
    
    return; 
};

void generateTrigArray(int res, char* out)
{
    int i = 0;
            
    for (i = 0; i < res; i++)
    {
        *out++= 255*i/res; //
    }
    
    return; 
};


void setVoltage(char channel, char voltage)
{
    // bit 6 5 4 are constant always (respectively 0 1 1)
    char data1 = 0b00110000, data2 = 0;
    char v_high;
    data1 |= channel << 7;
    
    v_high  = voltage >> 4;       // shift out
    v_high  &= 0b00001111;        // clear first four bits; 
    
    data1   += v_high;
    data2   = voltage << 4; 
    
    CS = 0; // pull down chip select
    spi_io(data1);
    spi_io(data2);
    CS = 1; 
    
}