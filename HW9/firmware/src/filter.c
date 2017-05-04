#include "filter.h"


void filter_init()
{
    int i = 0;
    for (i = 0;i<MAF_WIN;i++)
    {
        mafPrev [i] = 0;
    }
    for (i = 0;i<FIR_WIN;i++)
    {
        firPrev [i] = 0;
    }
    
}
int  updateMAF (int in)
{
    int out= 0, i  = 0;
    for (i = 0; i< MAF_WIN-1; i++)
    {
        mafPrev[i] = mafPrev[i+1];// shift the buffer 
        out += mafPrev[i];
    }
    mafPrev[i] = in;
    out += in; 
    
    return (out / MAF_WIN);
   
};

int  updateFIR (int in)
{
    int out = 0, i= 0;
    for (i = 0; i< FIR_WIN-1; i++)
    {
        firPrev[i] = firPrev[i+1];// shift the buffer 
        out += firPrev[i]*firWeight[i];
    }
    firPrev[i] = in;
    out += in * firWeight[i]; 
    
    return out; 
}

int  updateIIR (int in)
{
    int out = iirPrev[0]*iirWeight + in* (1-iirWeight);
    iirPrev[0] = out;
    return out; 
}

void updateIIRVector(int16_t *in, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
      *in =  iirPrev[i]*iirWeight + (*in) * (1-iirWeight);
      iirPrev[i] = *in;
      in ++;
    }   
}