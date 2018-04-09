/*
 * LedDriver.h
 *
 * Created: 2018/04/09 16:20:06
 *  Author: tak
 */ 


#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

typedef struct {
	void (*create)();
	void (*destroy)();
	void (*turnOn)();
	void (*turnOff)();
} ILedDriver;
extern ILedDriver LedDriver;

#endif /* LEDDRIVER_H_ */
