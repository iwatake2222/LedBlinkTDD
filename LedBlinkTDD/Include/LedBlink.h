/*
 * LedBlink.h
 *
 * Created: 2018/04/09 17:15:47
 *  Author: tak
 */ 


#ifndef LEDBLINK_H_
#define LEDBLINK_H_

#include "Driver/LedDriver/LedDriver.h"
#include "Driver/Timer0/Timer0.h"

void LedBlink_start(ILedDriver* ledDriver, ITimer0* timer0, int interval);
void LedBlink_stop();


#endif /* LEDBLINK_H_ */