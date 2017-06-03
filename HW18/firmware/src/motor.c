#include "motor.h"

void motor_init()
{
    // put these initializations in APP_Initialize()
    RPA0Rbits.RPA0R = 0b0101; // A0 is OC1
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 0; // A1 is the direction pin to go along with OC1

    RPB2Rbits.RPB2R = 0b0101; // B2 is OC4
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 0; // B3 is the direction pin to go along with OC
    
    T2CONbits.TCKPS = 2; // prescaler N=4 
    PR2 = PERIOD - 1; // 10kHz
    TMR2 = 0;
    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
    OC4CONbits.OCM = 0b110;
    OC1RS = 0; // max allowed value is 1119
    OC1R = 0; // read-only initial value
    OC4RS = 0; // max allowed value is 1119
    OC4R = 0; // read-only initial value
    T2CONbits.ON = 1;
    OC1CONbits.ON = 1;
    OC4CONbits.ON = 1;
    
    motor_set_speed(0,0,0);
    motor_set_speed(1,0,0);
}


void motor_set_speed(int motor, int pct, int dir)
{
       // somewhere in APP_Tasks(), probably in case APP_STATE_SCHEDULE_READ
       // when you read data from the host
    if (motor == 0) // first motor 
    {
       LATAbits.LATA1 = dir; // direction
       OC1RS = pct*PERIOD/100; // velocity, 50%
    }
    else
    {
       LATBbits.LATB3 = dir; // direction
       OC4RS = pct*PERIOD/100; // velocity, 50%
    }
}
