
#ifndef _DAC_H    /* Guard against multiple inclusion */
#define _DAC_H

#include <math.h>

void generateSinArray(int res, char* out);
void generateTrigArray(int res, char* out);
void outputTrigDAC(int freq, int res);
void outputSinDAC(int freq, int res);
void setVoltage(char channel, char voltage);


#endif /* _EXAMPLE_FILE_NAME_H */
