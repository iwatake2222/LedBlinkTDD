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

void LedBlink_cb()
{
	static uint16_t cnt = 0;
	static int state = 0;
	if(cnt++ > 1000) {
		if(state == 0) {
			LedDriver_turnOn();
			state = 1;
		} else {
			LedDriver_turnOff();
			state = 0;
		}
		cnt = 0;
	}

}

int main(void)
{
	Uart0_defaultInit();
	Uart0_sendText("\n\n======================\nHello\n");

#if 1
	extern void runTestTarget(int argc, const char * argv[]);
	const char * argv[] = {"main", "-v"};
	runTestTarget(sizeof(argv), argv);
#endif
	
	/* ToDo this code should go LedBlink */
	{
		Timer0_init();
		LedDriver_create();
		Timer0_setCallback(LedBlink_cb);	
	}

	while(1);
}
