#ifndef _FILTER_H    /* Guard against multiple inclusion */
#define _FILTER_H


#define MAF_WIN 4
#define FIR_WIN 4

// current state
int    mafData = 0;
int    firData = 0;
int    iirData = 0;

// initialize the fir weight here
float  firWeight[FIR_WIN] = {0.1, 0.2, 0.3, 0.4}; 

// buffer needed to store previous states
int mafPrev [MAF_WIN];
int firPrev [FIR_WIN];
int iirPrev = 0;

const float iirWeight = 0.2;

// update functions
void filter_init();
int  updateMAF (int in);
int  updateFIR (int in);
int  updateIIR (int in);


#endif 
/* *****************************************************************************
 End of File
 */
