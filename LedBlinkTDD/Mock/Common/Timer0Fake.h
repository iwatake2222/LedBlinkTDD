/*
 * Timer0Fake.h
 *
 * Created: 2018/04/09 18:17:05
 *  Author: tak
 */ 


#ifndef TIMER0FAKE_H_
#define TIMER0FAKE_H_

#include "Driver/Timer0/Timer0.h"

void Timer0Fake_reset();
int  Timer0Fake_getIsInit();
void Timer0Fake_setTime(int t);
void Timer0Fake_invokeIRQ();

extern ITimer0 Timer0Fake;




#endif /* TIMER0FAKE_H_ */