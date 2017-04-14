#include "tft.h"

// F0E2
int drawChar(int x, int y, char c, int color)
{  
    int i = 0,j = 0;
    drawRect(x,y,CHAR_WIDTH,CHAR_HEIGHT,BACKGROUND); // clear previous buffer
            
//    for (i = x;i<x+CHAR_HEIGHT;i++) // clear the bar for writing the following sentence
//        clearRowBuff(i);
    
    if (((x+CHAR_WIDTH) > IMG_WIDTH)||((y+CHAR_HEIGHT)>IMG_HEIGHT))
    {
        // output error msg (TURN OFF GREEN LED)
        screenOverFlow();
        return 0; 
    }
 
    for (i = 0; i < CHAR_WIDTH;i++) // 0 to 4
        for (j = 0;j < CHAR_HEIGHT;j++)
        {
            if (ASCII[c-0x20][i] & (1 << j))   
                LCD_drawPixel(x+i,y+j,color);
        }
    return 1;
};


int drawStr (int x, int y, char* str, int color)
{
    int len = strlen(str);
    int i = 0;
    
    if (((x+len*CHAR_WIDTH) > IMG_WIDTH)||((y+CHAR_HEIGHT)>IMG_HEIGHT))
    {
        screenOverFlow(); //toggle led
        return 0; 
    }
   
    for (i = 0;i<len;i++)
        drawChar(x+i*CHAR_WIDTH , y, str[i], color);
    
    return 0;
}

int drawRect(int x, int y, int width, int height, int color)
{
    int i = 0,j = 0; 
    
    if (((x+width) > IMG_WIDTH)||((y+height)>IMG_HEIGHT))
    {
        screenOverFlow(); //toggle led
        return 0; 
    }
    
//    if (width > 0)
//    {
    for (i = x; i < x+ width; i++)
        for (j = y; j < y + height; j++)
            LCD_drawPixel(i,j,color);
//    }
//    else
//    {
//       for (i = x+width; i < x; i++)
//        for (j = y; j < y + height; j++)
//            LCD_drawPixel(i,j,color); 
//    }
    
}

void clearRowBuff(int x)
{
    int i = 0;
    for (i = 0; i<IMG_WIDTH;i++)
        LCD_drawPixel(i,x,BACKGROUND);
};

void screenOverFlow()
{        
    // output error msg (TOGGLE GREEN LED)
        PORTACLR = 0x0010;
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT()<23999999)
        {
            ;
        }
        PORTASET = 0x0010;
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT()<23999999)
        {
            ;
        }
        PORTACLR = 0x0010;    
}