/*
 * LedDriverTestHost.c
 *
 * Created: 2018/04/09 16:10:59
 *  Author: tak
 */ 
#include "Unity/unity_fixture.h"
#include "Wrapper/Avr_wrapper.h"
#include "Driver/LedDriver/LedDriver.h"

TEST_GROUP(LedDriverTestHost);

TEST_SETUP(LedDriverTestHost)
{
	LedDriver.create();
	Avr_wrapper_io_fake_setLastVal(0);
}

TEST_TEAR_DOWN(LedDriverTestHost)
{
}

TEST(LedDriverTestHost, create)
{
	/* DDR of LED(PB5) should be set to 1 */
	LedDriver.create();
	TEST_ASSERT_TRUE(Avr_wrapper_io_fake_getLastVal() & (1 << 5));
}

TEST(LedDriverTestHost, destroy)
{
	/* Nothing to check */
	LedDriver.destroy();
}

TEST(LedDriverTestHost, turnOn)
{
	LedDriver.turnOn();
	TEST_ASSERT_TRUE(Avr_wrapper_io_fake_getLastVal() & (1 << 5));
}

TEST(LedDriverTestHost, turnOff)
{
	Avr_wrapper_io_fake_setLastVal(0xFF);
	LedDriver.turnOff();
	TEST_ASSERT_FALSE(Avr_wrapper_io_fake_getLastVal() & (1 << 5));
}

TEST_GROUP_RUNNER(LedDriverTestHost)
{
	RUN_TEST_CASE(LedDriverTestHost, create);
	RUN_TEST_CASE(LedDriverTestHost, destroy);
	RUN_TEST_CASE(LedDriverTestHost, turnOn);
	RUN_TEST_CASE(LedDriverTestHost, turnOff);
}
