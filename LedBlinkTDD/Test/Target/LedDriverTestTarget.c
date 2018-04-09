/*
 * LedDriverTestTarget.c
 *
 * Created: 2018/04/09 16:10:59
 *  Author: tak
 */ 
#include "Unity/unity_fixture.h"
#include "Driver/LedDriver/LedDriver.h"
#include "Driver/Uart0/Uart0.h"

TEST_GROUP(LedDriverTestTarget);

TEST_SETUP(LedDriverTestTarget)
{
	LedDriver.create();
}

TEST_TEAR_DOWN(LedDriverTestTarget)
{
	LedDriver.destroy();
}

TEST(LedDriverTestTarget, on)
{
	Uart0_sendText("Turn on LED. Please check, then press any key\n");
	LedDriver.turnOn();
	Uart0_recv();
	
}

TEST(LedDriverTestTarget, off)
{
	Uart0_sendText("Turn off LED. Please check, then press any key\n");
	LedDriver.turnOff();
	Uart0_recv();
}



TEST_GROUP_RUNNER(LedDriverTestTarget)
{
	RUN_TEST_CASE(LedDriverTestTarget, on);
	RUN_TEST_CASE(LedDriverTestTarget, off);
}
