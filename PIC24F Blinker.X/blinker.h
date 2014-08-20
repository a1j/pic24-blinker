/* 
 * File:   blinker.h
 * Author: Eugene Aleynikov
 *
 * Created on August 12, 2014, 10:21 PM a1j@users.noreply.github.com
 */

#ifndef BLINKER_H
#define	BLINKER_H

#ifdef	__cplusplus
extern "C" {
#endif

void initMCU(void);
void initLed(void);
void resetTimer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BLINKER_H */

