/*
 * Timer0.h
 *
 * Created: 2018/04/09 16:56:22
 *  Author: tak
 */ 
#ifndef TIMER0_H_
#define TIMER0_H_


/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/

/***
 * Struct definitions
 ***/

/***
 * External APIs
 ***/


typedef struct {
	void (*init)();
	uint16_t (*getTimeMS)();
	RET (*setCallback)(void (*func)());
	RET (*clearCallback)(void (*func)());
} ITimer0;
extern ITimer0 Timer0;


#endif /* TIMER0_H_ */