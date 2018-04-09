/*
 * Timer0.c
 *
 * Created: 2018/04/09 16:56:29
 *  Author: tak
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Common.h"
#include "Driver/Timer0/Timer0.h"

/*** Internal Const Values ***/

/*** Internal Static Variables ***/
// over flow happens every 65.536 sec
static volatile uint16_t s_cntTimer0_1ms = 0;
static void (*s_callback)();

/*** Internal Function Declarations ***/

/*** External Function Defines ***/
void Timer0_init()
{
	// CTC Mode without compare output
	TCCR0A = (0<<COM0A0) | (0<<COM0B0) | (2<<WGM00);
	// Div = 1/64
	TCCR0B = 0x03;
	// Compare match every 1 msec (1/ (64/16M) / 1000)
	OCR0A = F_CPU / 64 / 1000;
	// Enable Compare Match A Interrupt
	TIMSK0 = (1<<OCIE0A);
}

RET Timer0_setCallback(void (*func)())
{
	if(s_callback == 0){
		s_callback = func;
		return RET_OK;
	}
	return RET_ERR_REGISTER;
	
}

RET Timer0_clearCallback(void (*func)())
{
	if(s_callback == func){
		s_callback = 0;
		return RET_OK;
	}
	return RET_ERR_REGISTER;
}

// over flow happens every 65.536 sec
uint16_t Timer0_getTimeMS()
{
	return s_cntTimer0_1ms;
}

ITimer0 Timer0 = {
	.init = Timer0_init,
	.getTimeMS = Timer0_getTimeMS,
	.setCallback = Timer0_setCallback,
	.clearCallback = Timer0_clearCallback,
};

/*** Internal Function Definitions ***/
ISR(TIMER0_COMPA_vect)
{
	s_cntTimer0_1ms++;
	if(s_callback != 0) s_callback();
	return;
}