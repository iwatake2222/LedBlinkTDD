/*
 * Timer0Fake.c
 *
 * Created: 2018/04/09 18:17:13
 *  Author: tak
 */
#include "Common.h"
#include "Timer0Fake.h"

/*****************************************
 * Data containers and accessors
 *****************************************/
static int s_isInit = -1;
static uint16_t s_cntTimer0_1ms = 0;
static void (*s_callback)() = 0;

void Timer0Fake_reset()
{
	s_isInit = -1;
	s_cntTimer0_1ms = 0;
	s_callback = 0;
}

int Timer0Fake_getIsInit()
{
	return s_isInit;
}

void Timer0Fake_setTime(int t)
{
	s_cntTimer0_1ms = t;	
}

void Timer0Fake_invokeIRQ()
{
	s_cntTimer0_1ms++;
	if(s_callback != 0) s_callback();
}

/*****************************************
 * Mock functions
 *****************************************/
static void Timer0Fake_init()
{
	s_isInit = 1;
}

static RET Timer0Fake_setCallback(void (*func)())
{
	s_callback = func;
	return RET_OK;
}

static RET Timer0Fake_clearCallback(void (*func)())
{
	s_callback = 0;
	return RET_OK;
}

static uint16_t Timer0Fake_getTimeMS()
{
	return s_cntTimer0_1ms;
}

ITimer0 Timer0Fake = {
	.init = Timer0Fake_init,
	.getTimeMS = Timer0Fake_getTimeMS,
	.setCallback = Timer0Fake_setCallback,
	.clearCallback = Timer0Fake_clearCallback,
};