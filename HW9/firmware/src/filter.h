#ifndef _FILTER_H    /* Guard against multiple inclusion */
#define _FILTER_H
#include <stdint.h>


#define MAF_WIN 6
#define FIR_WIN 6

// current state
int    mafData = 0;
int    firData = 0;
int    iirData [3]= {0,0,0};

// initialize the fir weight here
//float  firWeight[FIR_WIN] = {0.0152, 0.126, 0.3588, 0.3588, 0.126, 0.0152}; // cutoff at 0.25
float firWeight[FIR_WIN]  = {0.0264, 0.1405, 0.3331, 0.3331,   0.1405,   0.0264}; //cutoff at 0.1


// buffer needed to store previous states
int mafPrev [MAF_WIN];
int firPrev [FIR_WIN];
int16_t iirPrev [3] = {0,0,0};

const float iirWeight = 0.1;

// update functions
void filter_init();
int  updateMAF (int in);
int  updateFIR (int in);
int  updateIIR (int in);
void updateIIRVector(int16_t *in, int len);

#endif 
/* *****************************************************************************
 End of File
 */
