/*
 * LedDriverFake.h
 *
 * Created: 2018/04/09 17:55:25
 *  Author: tak
 */ 


#ifndef LEDDRIVERFAKE_H_
#define LEDDRIVERFAKE_H_

#include "Driver/LedDriver/LedDriver.h"

void LedDriverFake_reset();
int LedDriverFake_getIsInit();
int LedDriverFake_getStatus();

extern ILedDriver LedDriverFake;


#endif /* LEDDRIVERFAKE_H_ */