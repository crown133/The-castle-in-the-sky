#include "adc.h"

/**********************************************
DATA: 2018/6/6

 Coded by LZ
*************************************************************************/
#define ADC_SAMPLE_NUM	10

u16 adc_value[3*ADC_SAMPLE_NUM];//ADC采集值存放缓冲区



//初始化ADC，使用DMA传输
//通道PA1\PA2\PA3
void Adc_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//使ADC1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA时钟
	
	//PA1\2\3 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//DMA 配置
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;	 //ADC1->DR地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc_value;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;    //从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 3*ADC_SAMPLE_NUM;  //DMA通道的DMA缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址增加
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //非内存到内存传输关闭
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	  //扫描模式，用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 3;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC1, ENABLE);//使能ADC1 DMA
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	//配置连续转换通道，55.5个采样周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	//1个通道转换一次耗时21us 4个通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);	//采样个数ADC_SAMPLE_NUM
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);	//总共耗时3*21*ADC_SAMPLE_NUM（64）=
	
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
 
//ADC均值滤波
void ADC_Filter(u16* adc_val)
{
	u16 i=0;
	u32 sum[3]={0,0,0};
	
	for(;i<ADC_SAMPLE_NUM;i++)
	{
		sum[0]+=adc_value[3*i+0];
		sum[1]+=adc_value[3*i+1];
		sum[2]+=adc_value[3*i+2];
	}
	adc_val[0]=sum[0]/ADC_SAMPLE_NUM;
	adc_val[1]=sum[1]/ADC_SAMPLE_NUM;
	adc_val[2]=sum[2]/ADC_SAMPLE_NUM;
}

/*****定时器中断，每10ms中断一次，ADC采样*/
/*void ADC_IT(u8 arr, u8 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断待处理位
	
	TIM_DeInit(TIM3);
	TIM3_TimeBaseStructure.TIM_Period            = arr;
	TIM3_TimeBaseStructure.TIM_Prescaler         = psc;
	TIM3_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM3_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	 {
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
     }
	  ADC_Filter(adc_after_val);
	  Check_Val = adc_after_val[0];
	  Cir2_PID.fdb = adc_after_val[1];
	  Cir1_PID.fdb = adc_after_val[2];
}
*/


