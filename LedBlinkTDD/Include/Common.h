/*
 * common.h
 *
 * Created: 2018/04/09 9:24:40
 *  Author: tak
 */ 

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stddef.h>

/***
 * Const values
 ***/
#define F_CPU 16000000

/***
 * Macros
 ***/
#define SET_BIT(PORT, BIT) (PORT |=  (1<<BIT))
#define CLR_BIT(PORT, BIT) (PORT &= ~(1<<BIT))
#define SET_VAL(PORT, START_BIT, SIZE, VAL) (PORT = (PORT & ~(((1<<SIZE)-1) << START_BIT)) | (VAL << START_BIT))
#define IS_SET(PORT, BIT) ( (PORT&(1<<BIT)) == (1<<BIT) )

/***
 * Enum definitions
 ***/
typedef enum {
	RET_OK            = 0,
	RET_WAR_NO_RESULT = 1,
	RET_ERR           = -1,
	RET_ERR_PRM       = -2,
	RET_ERR_CONFLICT  = -3,
	RET_ERR_REGISTER  = -4,
} RET;

#endif /* COMMON_H_ */