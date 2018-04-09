/*
 * LedBlink.c
 *
 * Created: 2018/04/09 17:16:04
 *  Author: tak
 */ 
#include "Common.h"
#include "LedBlink.h"

static ILedDriver * s_ledDriver;
static ITimer0 * s_timer0;
static int s_interval;

static void callback()
{
	static uint16_t cnt = 1;
	static int state = 0;
	if(cnt++ >= s_interval) {
		if(state == 0) {
			s_ledDriver->turnOn();
			state = 1;
		} else {
			s_ledDriver->turnOff();
			state = 0;
		}
		cnt = 1;
	}

}

void LedBlink_start(ILedDriver* ledDriver, ITimer0* timer0, int interval)
{
	s_ledDriver = ledDriver;
	s_timer0 = timer0;
	s_interval = interval;

	s_ledDriver->create();
	s_ledDriver->turnOff();
	s_timer0->init();
	s_timer0->setCallback(callback);
}

void LedBlink_stop()
{
	s_ledDriver->destroy();
}
