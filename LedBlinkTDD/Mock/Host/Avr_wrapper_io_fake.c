#include "Wrapper/Avr_wrapper.h"

int Avr_wrapper_io_fake_val = 0;

int Avr_wrapper_io_fake_getLastVal()
{
	return Avr_wrapper_io_fake_val;
}

void Avr_wrapper_io_fake_setLastVal(int val)
{
	Avr_wrapper_io_fake_val = val;
}
