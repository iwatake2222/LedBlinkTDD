/*
 * LedBlinkTDD.c
 *
 * Created: 2018/04/09 8:19:39
 * Author : tak
 */
#include "Common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Uart0/Uart0.h"

int main(void)
{
	Uart0_defaultInit();
	Uart0_sendText("\n\n======================\nHello\n");

#if 1
	extern void runTestTarget(int argc, const char * argv[]);
	const char * argv[] = {"main", "-v"};
	runTestTarget(sizeof(argv), argv);
#endif

	DDRB = 0xff;
	while (1) {
		PORTB = 0;
		// (*(volatile uint8_t *)((0x05) + 0x20)) = 0;
		_delay_ms(1000);
		 PORTB = 0xFF;
		// (*(volatile uint8_t *)((0x05) + 0x20)) = 0xFF;
		_delay_ms(1000);
	}
}


