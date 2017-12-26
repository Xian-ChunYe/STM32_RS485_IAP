#include "sys.h"
#include "usart.h"	  
///////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;
}
#endif 


void uart_init(u32 bordrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_instructure;
	 
    /******************时钟初始化**********************/	
	/* 打开 GPIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
	/* 打开 UART 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    
    /******************引脚初始化**********************/
	/* 配置 USART  IO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  		 //发送
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	//复用推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			// 接收
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    USART3_RX;
    
    /******************串口初始化**********************/
    USART_DeInit(USART3);			   //串口外设复位

	USART_instructure.USART_BaudRate=bordrate;												  //串口属性配置
	USART_instructure.USART_WordLength=USART_WordLength_8b;
	USART_instructure.USART_StopBits=USART_StopBits_1;
	USART_instructure.USART_Parity=USART_Parity_No;
	USART_instructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_instructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    
	USART_Init(USART3,&USART_instructure);

    USART_Cmd(USART3, ENABLE);                    //使能串口3 
}

/**
  * @brief  Print a character on the HyperTerminal
  * @param  c: The character to be printed
  * @retval None
  */
void SerialPutChar(uint8_t c)
{    
  USART3_TX;
    
  USART_SendData(USART3, c);  
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}
       
  USART3_RX;
}

/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
void Serial_PutString(uint8_t *s)
{    
  while (*s != '\0')
  {
    SerialPutChar(*s);
    s++;
  }
}
