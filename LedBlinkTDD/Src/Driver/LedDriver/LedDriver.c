/*
 * LedDriver.c
 *
 * Created: 2018/04/09 16:19:20
 *  Author: tak
 */ 
#include "Common.h"
#include "Wrapper/Avr_wrapper.h"
#include "Driver/LedDriver/LedDriver.h"

static void LedDriver_create_impl()
{
	/* Set LED(PB5) as output */
	AVR_WRAPPER_IO(DDRB) |= 1 << 5;
}

static void LedDriver_destroy_impl()
{
}

static void LedDriver_turnOn_impl()
{
	AVR_WRAPPER_IO(PORTB) |= 1 << 5;
}

static void LedDriver_turnOff_impl()
{
	AVR_WRAPPER_IO(PORTB) &= ~(1 << 5);
}

ILedDriver LedDriver = {
	.create = LedDriver_create_impl,
	.destroy = LedDriver_destroy_impl,
	.turnOn = LedDriver_turnOn_impl,
	.turnOff = LedDriver_turnOff_impl,
};