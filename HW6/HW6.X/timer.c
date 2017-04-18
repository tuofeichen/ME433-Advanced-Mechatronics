#include <proc/p32mx250f128b.h>

#include "timer.h"

void timer_init() 
{
//  10Hz with 48Mhz Timer -> 48e6/10 = (PR+1)*PRE_SCALER 


    PR2 = 37499; //18749*2; 
    TMR2 = 0;                   // reset timer 2
    T2CONbits.TCKPS = 0b111;    // 256 prescaler
    T2CONbits.TGATE = 0;        // not gated
    T2CONbits.ON = 1;           // turn on
    IPC2bits.T2IP = 7;          // priority
    IPC2bits.T2IS = 0;          // sub-priority
    IFS0bits.T2IF = 0;          // clear interrupt flag
    IEC0bits.T2IE = 1;          // enable interrupt
}