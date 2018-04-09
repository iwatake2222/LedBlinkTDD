/*
 * LedBlinkTestCommon.c
 *
 * Created: 2018/04/09 16:10:59
 *  Author: tak
 */ 
#include "Common.h"
#include "Unity/unity_fixture.h"
#include "Mock/Common/LedDriverFake.h"
#include "Mock/Common/Timer0Fake.h"
#include "LedBlink.h"

TEST_GROUP(LedBlinkTestCommon);


TEST_SETUP(LedBlinkTestCommon)
{
	LedDriverFake_reset();
	Timer0Fake_reset();
}

TEST_TEAR_DOWN(LedBlinkTestCommon)
{
}

TEST(LedBlinkTestCommon, start)
{
	LedBlink_start(&LedDriverFake, &Timer0Fake, 1000);
	TEST_ASSERT_EQUAL(1, LedDriverFake_getIsInit());
	TEST_ASSERT_EQUAL(0, LedDriverFake_getStatus());
	TEST_ASSERT_EQUAL(1, Timer0Fake_getIsInit());
}

TEST(LedBlinkTestCommon, stop)
{
	LedBlink_stop();
}

TEST(LedBlinkTestCommon, blink)
{
	LedBlink_start(&LedDriverFake, &Timer0Fake, 1000);
	TEST_ASSERT_EQUAL(0, LedDriverFake_getStatus());
	for (int i = 0; i < 999; i++) {
		Timer0Fake_invokeIRQ();
	}
	TEST_ASSERT_EQUAL(0, LedDriverFake_getStatus());
	Timer0Fake_invokeIRQ();
	TEST_ASSERT_EQUAL(1, LedDriverFake_getStatus());
}


TEST_GROUP_RUNNER(LedBlinkTestCommon)
{
	RUN_TEST_CASE(LedBlinkTestCommon, start);
	RUN_TEST_CASE(LedBlinkTestCommon, stop);
	RUN_TEST_CASE(LedBlinkTestCommon, blink);
}
