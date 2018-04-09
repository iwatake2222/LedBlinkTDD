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
void Timer0_init();
uint16_t Timer0_getTimeMS();
RET Timer0_setCallback(void (*func)());
RET Timer0_clearCallback(void (*func)());


#endif /* TIMER0_H_ */