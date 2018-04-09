#include "Unity/unity_fixture.h"

extern int add(int a, int b);
extern int sub(int a, int b);

TEST_GROUP(SampleLogicTest);

TEST_SETUP(SampleLogicTest)
{
}

TEST_TEAR_DOWN(SampleLogicTest)
{
}

TEST(SampleLogicTest, add)
{
	TEST_ASSERT_EQUAL(3, add(1, 2));
}

TEST(SampleLogicTest, sub)
{
	TEST_ASSERT_EQUAL(1, sub(3, 2));
}

TEST_GROUP_RUNNER(SampleLogicTest)
{
	RUN_TEST_CASE(SampleLogicTest, add);
	RUN_TEST_CASE(SampleLogicTest, sub);
}
