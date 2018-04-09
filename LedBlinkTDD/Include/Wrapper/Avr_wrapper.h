/*
 * Avr_wrapper.h
 *
 * Created: 2018/04/09 15:36:56
 *  Author: tak
 */ 


#ifndef AVR_WRAPPER_H_
#define AVR_WRAPPER_H_

#ifndef UNIT_TEST_HOST

#include <avr/io.h>
#define AVR_WRAPPER_IO(X) X

#else

extern int Avr_wrapper_io_fake_val;
#define AVR_WRAPPER_IO(X) Avr_wrapper_io_fake_val
int Avr_wrapper_io_fake_getLastVal();
void Avr_wrapper_io_fake_setLastVal(int val);

#endif /* UNIT_TEST_HOST */

#endif /* AVR_WRAPPER_H_ */