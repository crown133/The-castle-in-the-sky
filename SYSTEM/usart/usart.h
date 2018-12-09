#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
 /**************************************************************************
 作  者 ：大鱼电子
淘宝地址：https://shop119207236.taobao.com
**************************************************************************/
void uart1_init(u32 bound);					//串口1初始化函数
void USART1_IRQHandler(void);     	//串口1中断服务程序
#endif


