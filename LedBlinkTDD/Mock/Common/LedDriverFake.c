/*
 * LedDriverFake.c
 *
 * Created: 2018/04/09 17:51:07
 *  Author: tak
 */ 
#include "LedDriverFake.h"

/*****************************************
 * Data containers and accessors
 *****************************************/
static int s_isInit = -1;
static int s_status = -1;

void LedDriverFake_reset()
{
	s_isInit = -1;
	s_status = -1;
}

int LedDriverFake_getIsInit()
{
	return s_isInit;
}

int LedDriverFake_getStatus()
{
	return s_status;
}

/*****************************************
 * Mock functions
 *****************************************/
static void LedDriverFake_create()
{
	s_isInit = 1;
}

static void LedDriverFake_destroy()
{
	s_isInit = 0;
}

static void LedDriverFake_turnOn()
{
	s_status = 1;
}

static void LedDriverFake_turnOff()
{
	s_status = 0;
}

ILedDriver LedDriverFake = {
	.create = LedDriverFake_create,
	.destroy = LedDriverFake_destroy,
	.turnOn = LedDriverFake_turnOn,
	.turnOff = LedDriverFake_turnOff,
};