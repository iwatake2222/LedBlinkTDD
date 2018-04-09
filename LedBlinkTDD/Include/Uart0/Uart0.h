/*
 * Uart0.h
 *
 * Created: 2018/04/09 9:17:03
 *  Author: tak
 */ 

#ifndef UART0_H_
#define UART0_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/
typedef enum {
	UART_OPEN_SPEED_9600    = 0,
	UART_OPEN_SPEED_19200   = 1,
	UART_OPEN_SPEED_38400   = 2,
	UART_OPEN_SPEED_115200  = 3,
	UART_OPEN_SPEED_1M      = 4,
	UART_OPEN_SPEED_2M      = 5,
} UART_OPEN_SPEED;

/***
 * Struct definitions
 ***/
typedef struct {
	UART_OPEN_SPEED speed;
} UART_OPEN_PRM;

/***
 * External APIs
 ***/
void Uart0_defaultInit();
void Uart0_open(UART_OPEN_PRM *prm);
void Uart0_close();
void Uart0_send(uint8_t data);
void Uart0_sendText(const char * text);
uint8_t Uart0_isRecved();
uint8_t Uart0_recv();

#endif /* UART0_H_ */
