/*
 * AllTestTarget.c
 *
 * Created: 2018/04/09 10:15:02
 *  Author: tak
 */ 
#include "Unity/unity_fixture.h"
#include "Uart0/Uart0.h"

static void AllTestTarget(void)
{
	RUN_TEST_GROUP(SampleLogicTest);
}

void runTestTarget(int argc, const char * argv[])
{
	extern void AllTestCommon(void);
	UnityMain(argc, argv, AllTestCommon);

	UnityMain(argc, argv, AllTestTarget);
}

/* Set outputChar for Unity */
/* Do not forget to define UNITY_OUTPUT_CHAR and UNITY_OUTPUT_CHAR_HEADER_DECLARATION */
void (*outputChar)(char) = Uart0_send;
