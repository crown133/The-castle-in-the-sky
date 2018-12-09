#include "sys.h"


int main()
 {  
	delay_init();
	Adc_Init();
	//ADC_IT((u8)1000, (u8)7199);  //ADC中断采集
	Motor_gpio_Init();
	TIM1_PWM_Init((u8)7199, 0);
	//Control_Out((u8)100, (u8)7199); //频率72Mhz,72M/7200=10K，1ms采集一次中断
	 
	//Cir1_PID = Circle_Pid_Default;
	//Cir2_PID = Circle_Pid_Default;
/********线圈PID参数初始化，调参主要调此部分*********/

	Cir1_PID.ref  = 3080;   //线圈中位值
	Cir1_PID.kp   = 10.0f; //10
	Cir1_PID.ki   = 0.0f;
	Cir1_PID.kd   = 0.0f;
	
	Cir2_PID.ref  = 3100;
	Cir2_PID.kp   = 10.0f;
	Cir2_PID.ki   = 0.0f;
	Cir2_PID.kd   = 0.0f;
/*************************************************/
	while(1)
	{  
	  Circle_Control(A, 0, 5000);
	  Circle_Control(B, 0, 5000);
	}
	
 }
