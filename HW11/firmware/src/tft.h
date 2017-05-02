#ifndef _TFT_H    /* Guard against multiple inclusion */
#define _TFT_H
#include "ILI9163C.h"
#include <xc.h>  
#include <stdio.h>

#define BACKGROUND  0x0000 // keep background black 


#define IMG_HEIGHT 128
#define IMG_WIDTH  128
#define CHAR_HEIGHT 8
#define CHAR_WIDTH  5

int drawRect(int x , int y, int width, int height, int color);
int drawStr (int x, int y, char* str, int color);
int drawChar(int x, int y, char a, int color);
void clearRowBuff(int row);
void screenOverFlow();

 

#endif /* _EXAMPLE_FILE_NAME_H */

