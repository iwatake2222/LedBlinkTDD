/*
 * LedDriver.c
 *
 * Created: 2018/04/09 16:19:20
 *  Author: tak
 */ 
#include "Common.h"
#include "Wrapper/Avr_wrapper.h"
#include "Driver/LedDriver/LedDriver.h"

void LedDriver_create()
{
	/* Set LED(PB5) as output */
	AVR_WRAPPER_IO(DDRB) |= 1 << 5;
}

void LedDriver_destroy()
{
}

void LedDriver_turnOn()
{
	AVR_WRAPPER_IO(PORTB) |= 1 << 5;
}

void LedDriver_turnOff()
{
	AVR_WRAPPER_IO(PORTB) &= ~(1 << 5);
}
