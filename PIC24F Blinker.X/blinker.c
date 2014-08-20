/* 
 * File:   blinker.c
 * Author: Eugene Aleynikov
 *
 * Created on August 12, 2014, 10:28 PM
 */

// #include <stdio.h>
// #include <stdlib.h>
#include <xc.h>

/* MCU configuration bits */
_CONFIG1
(
   JTAGEN_OFF     // JTAG Disabled
   & GCP_OFF      // Code Protect Disabled
   & GWRP_OFF     // Write Protect Disabled
   & FWDTEN_OFF   // Watchdog Timer Disabled
   & WINDIS_OFF   // Windowed Watchdog Timer Disabled
)

_CONFIG2
(
   IESO_ON          // Two Speed Start-up
   //& PLL96MHZ_ON    // 96MHz PLL Enabled
   & PLLDIV_DIV3    //input clock divided by 3 to get 4MHz
   & FNOSC_PRIPLL   //FNOSC_FRC //FNOSC_PRIPLL   //primary clock with PLL
   & POSCMOD_HS     //POSCMOD_NONE //POSCMOD_HS
)
_CONFIG3
(
  //SOSCSEL_IO
  SOSCSEL_EC
)
//_CONFIG4
//(
//  RTCOSC_LPRC           //RTCC Source
//)

/* Timer1 period for 1 ms with FOSC = 4 MHz */
#define TMR1_PERIOD 0x03E8
#define ON_TIME  0x0200
#define OFF_TIME  0x0400
#define LED_ON()                (LATD |= (1U << 2))
#define LED_OFF()               (LATD &= ~(1U << 2))
#define LED_TOGGLE()            (LATD ^= (1U << 2))

struct state {
    unsigned int timer;
    unsigned int led_on;
} volatile context;

void initMCU(void) {
    T2CON = 0x0000U;  /* Use Internal Osc (Fcy), 16 bit mode, prescaler = 1 */
     // PIC24FJ256GB206 doesn't have port A
        TRISB = 0;
        TRISC = 0;
        TRISD = 0;
        TRISE = 0;
        TRISF = 0;
        TRISG = 0;
        //ODCBbits.ODB9 = 1;    //open-drain
        PORTB = 0;
        //VBUS_ON();
        //AD1PCFGL = 0xffff;    //analog off

    // PIC24FJ256GB206 turns off analog differently
        ANSB = 0;
        ANSC = 0;
        ANSD = 0;
        //ANSE = 0;
        ANSF = 0;
        ANSG = 0;
        //AD1PCFGbits.PCFG10 = 1;

        //Set serial console pins
        //PPSUnLock;
        //PPSOutput(PPS_RP5, PPS_U2TX);
        //PPSInput(PPS_U2RX, PPS_RP6);
        //PPSLock;
}
void resetTimer(void) {
    context.timer = 0;
    context.led_on = 0;
    TMR1 = 0;                   /* Reset timer*/
    PR1 = TMR1_PERIOD;
    T1CONbits.TCS = 0;          /* set int clock */
    IPC0bits.T1IP = 4;          /* sset prio level */
    IFS0bits.T1IF = 0;          /* Clear int flag */
    IEC0bits.T1IE = 1;          /* En. Interrupts*/

    SRbits.IPL = 3;             /* enable CPU priority levels 4-7*/
    T1CONbits.TON = 1;          /* start the timer*/
}
/*${ISR::_AddressError} ....................................................*/
void __attribute__((__interrupt__,__no_auto_psv__)) _AddressError(void) {
    while(1);    //Address error
}
/*${ISR::_StackError} ......................................................*/
void __attribute__((__interrupt__,__no_auto_psv__)) _StackError(void) {
    while(1);
}

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void) {
    if (context.timer > 0)
        context.timer -= 1; /* decrement it */
    else {
        if (context.led_on == 1 ) {
            context.timer = OFF_TIME;
            context.led_on = 0;
            LED_OFF();
        } else {
            context.timer = ON_TIME;
            context.led_on = 1;
            LED_ON();
        }
    }    
    IFS0bits.T1IF = 0; /* clear interrupt flag */
    return;
}


void initLed() {

}

int main(int argc, char** argv) {
    initMCU();
    initLed();
    resetTimer();
    while(1) {}
    return(0);
}

