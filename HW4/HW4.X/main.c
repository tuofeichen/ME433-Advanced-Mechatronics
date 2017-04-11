#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "spi.h"
#include "dac.h"

// DEVCFG0
#pragma config DEBUG  = OFF         // no debugging
#pragma config JTAGEN = OFF         // no jtag
#pragma config ICESEL = ICS_PGx1    // use PGED1 and PGEC1
#pragma config PWP    = OFF         // no write protect
#pragma config BWP    = OFF         // no boot write protect
#pragma config CP     = OFF         // no code protect

// DEVCFG1
#pragma config FNOSC    = PRIPLL       // use primary oscillator with pll
#pragma config FSOSCEN  = OFF          // turn off secondary oscillator
#pragma config IESO     = OFF          // no switching clocks
#pragma config POSCMOD  = HS           // high speed crystal mode
#pragma config OSCIOFNC = OFF          // free up secondary osc pins
#pragma config FPBDIV   = DIV_1        // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM    = CSDCMD       // do not enable clock switch
#pragma config WDTPS    = PS1048576    // slowest wdt
#pragma config WINDIS   = OFF          // no wdt window
#pragma config FWDTEN   = OFF          // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25    // wdt window at 25%

// DEVCFG2 - get the CPU clock to 48MHz
#pragma config FPLLIDIV = DIV_2     // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL  = MUL_24    // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2     // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2     // divider for the 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN   = ON        // USB clock on

// DEVCFG3
#pragma config USERID       = 0x0000  // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY     = OFF     // allow multiple reconfigurations
#pragma config IOL1WAY      = OFF     // allow multiple reconfigurations
#pragma config FUSBIDIO     = ON      // USB pins controlled by USB module
#pragma config FVBUSONIO    = ON      // USB BUSON controlled by USB module

#define  SAMPLE  100


volatile int cnt_sin = 0, cnt_trig = 0;
volatile char trigWave[SAMPLE], sinWave[SAMPLE];

void __ISR(_TIMER_2_VECTOR, IPL7SOFT)blink(void)
{
        cnt_sin  ++; 
        cnt_trig ++;
        const char period = 100; 
        
        if (cnt_sin%period)
            setVoltage(0,sinWave[cnt_sin/period]);
            
        if (cnt_trig%(period*2))
            setVoltage(1,trigWave [cnt_trig/(period*2)]);
         

        if (cnt_sin >= period  * 100) // has nothing to do with the cycle
                    cnt_sin = 0; //reset counter 
           
        if (cnt_trig >= period * 2* 100) // has nothing to do with the cycle
                    cnt_trig = 0; //reset counter 
        
    IFS0bits.T1IF = 0;          // clear interrupt flag
};

int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0; //set A4 to be output (Green LED)
    TRISBbits.TRISB4 = 1; //set B4 to be input  (debug button)
    _CP0_SET_COUNT(0);
    
    spi_init();
    timer_init();
    
    __builtin_enable_interrupts();


    generateTrigArray(SAMPLE, trigWave);
    generateSinArray (SAMPLE, sinWave);
   
    
    while(1) {
        
	    // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
		  // remember the core timer runs at half the CPU speed
        /*
        if (_CP0_GET_COUNT() > 24000 ) // blink at 1khz square wave
        {
            cnt_sin  ++; 
            cnt_trig ++;
           
            setVoltage(0,sinWave[cnt_sin]);
            
            if (cnt_trig%2)
                setVoltage(1,trigWave [cnt_trig/2]);
         

            if (cnt_sin >= 100) // has nothing to do with the cycle
                    cnt_sin = 0; //reset counter 
           
            if (cnt_trig >= 200) // has nothing to do with the cycle
                    cnt_trig = 0; //reset counter 
                
            PORTAINV = 0x0010;
            _CP0_SET_COUNT(0);
        }
        */

        if (!PORTBbits.RB4)
        {
            PORTACLR = 0x0010;
        }
        
    }
}