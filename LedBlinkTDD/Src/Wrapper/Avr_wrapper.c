/*
 * Avr_wrapper.c
 *
 * Created: 2018/04/09 15:47:42
 *  Author: tak
 */
#include <avr/interrupt.h>
#include "Wrapper/Avr_wrapper.h"

void Avr_wrapper_enableIrq()
{
	sei();
}

void Avr_wrapper_disableIrq()
{
	cli();
}