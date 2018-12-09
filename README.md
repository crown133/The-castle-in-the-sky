# The-castle-in-the-sky
这是一个基于stm32f103单片机的下推式磁悬浮项目
在天空之城中的那个姑娘，曾经是我的向往

驱动：TB6612fng

主控：STM32 f103c8t6

ADC: PA1--ADC12_IN1 用于检测浮子是否放上，避免功率过大烧毁驱动
     PA2--ADC12_IN2 得到浮子位置
     PA3--ADC12_IN3 得到浮子位置

PWM：PWMB--PA8 
     BIN1--PB15
     BIN2--PB14
     
     PWMA--PA11
     AIN1--PB12
     AIN2--PB13

CIRCLE:
     AO1--Circle1
     AO2--Circle2
     BO1--Circle3
     BO2--Circle4
