/*
 * LedBlinkTestCommon.c
 *
 * Created: 2018/04/09 16:10:59
 *  Author: tak
 */ 
#include "Common.h"
#include "Unity/unity_fixture.h"
#include "Driver/LedDriver/LedDriver.h"
#include "Driver/Timer0/Timer0.h"
#include "LedBlink.h"

TEST_GROUP(LedBlinkTestCommon);

TEST_SETUP(LedBlinkTestCommon)
{
}

TEST_TEAR_DOWN(LedBlinkTestCommon)
{
}

TEST(LedBlinkTestCommon, start)
{
	/* todo */
	LedBlink_start();
}

TEST(LedBlinkTestCommon, stop)
{
	/* todo */
	LedBlink_stop();
}



TEST_GROUP_RUNNER(LedBlinkTestCommon)
{
	RUN_TEST_CASE(LedBlinkTestCommon, start);
	RUN_TEST_CASE(LedBlinkTestCommon, stop);
}
