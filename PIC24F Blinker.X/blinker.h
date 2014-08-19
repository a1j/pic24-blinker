/* 
 * File:   blinker.h
 * Author: Eugene Aleynikov
 *
 * Created on August 12, 2014, 10:21 PMa1j@users.noreply.github.com
 */

#ifndef BLINKER_H
#define	BLINKER_H

#ifdef	__cplusplus
extern "C" {
#endif

void __attribute__((__interrupt__,__no_auto_psv__)) _AddressError(void);
void __attribute__((__interrupt__,__no_auto_psv__)) _StackError(void);
void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void);
void initMCU(void);
void initLed(void);
void resetTimer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BLINKER_H */

