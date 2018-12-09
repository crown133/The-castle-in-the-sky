#include "pwm.h"

void Motor_gpio_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin       = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	Out_AIN1=0, Out_AIN2=0;
	Out_BIN1=0, Out_BIN2=0;
}

/*****************************
pwm 输出初始化
自动重装值7200 /pwm频率10KHz 
预分频为0，不分频
*****************************/
void TIM1_PWM_Init(u8 arr, u8 psc)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef 	    TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_11;  //A PA11 / B PA8
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);       //PWM输出使能
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CCR寄存器预装载
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_Cmd(TIM1, ENABLE);
}
/*************************************
控制函数，输入为线圈组A(线圈1 2) | B线圈（3 4)，线圈正反向，PWM值 输入为其它值则关闭
**************************************/
void Circle_Control(int cir, int dir, u16 pwm_val)
 {
	if(1 == cir) //Circle B
	{
		if(dir) //dir2
		{
			Out_BIN1 = 1;
			Out_BIN2 = 0;
			Pwm_VB = pwm_val;
		}
		else   //dir1
		{
			Out_BIN1 = 0;
			Out_BIN2 = 1;
			Pwm_VB = pwm_val;
		}
	}
	else if(0 == cir)  //Circle A
	{
		if(dir) //dir2
		{
			Out_AIN1 = 0;
			Out_AIN2 = 1;
			Pwm_VA = pwm_val;
		}
		else 
		{
			Out_AIN1 = 1;
			Out_AIN2 = 0;
			Pwm_VA = pwm_val;
		}
	}
	else 
	{
		Out_AIN1 = 0;
	    Out_AIN2 = 0;
		Pwm_VA = 0;
		
		Out_BIN1 = 0;
		Out_BIN2 = 0;
		Pwm_VB = 0;
	}
 }
 







