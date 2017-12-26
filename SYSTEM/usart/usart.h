#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
	  	
#define  USART3_TX  	    GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define  USART3_RX 		    GPIO_ResetBits(GPIOC,GPIO_Pin_9)

void Serial_PutString(uint8_t *s);
void SerialPutChar(uint8_t c);
void uart_init(u32 bound);

#endif


