/*
 * LedBlinkTDD.c
 *
 * Created: 2018/04/09 8:19:39
 * Author : tak
 */
#include "Common.h"
#include "Driver/Uart0/Uart0.h"
#include "Wrapper/Avr_wrapper.h"
#include "Driver/LedDriver/LedDriver.h"
#include "Driver/Timer0/Timer0.h"


int main(void)
{
	Uart0_defaultInit();
	Uart0_sendText("\n\n======================\nHello\n");

#if 1
	extern void runTestTarget(int argc, const char * argv[]);
	const char * argv[] = {"main", "-v"};
	runTestTarget(sizeof(argv), argv);
#endif
	
	
	LedBlink_start(&LedDriver, &Timer0, 1000);

	while(1);
}
