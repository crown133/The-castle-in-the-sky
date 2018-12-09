#ifndef   _PWM_H_
#define   _PWM_H_

#include "sys.h"

#define  Pwm_VB        TIM1->CCR1   //PA8
#define  Pwm_VA        TIM1->CCR4   //PA11
         
#define  Out_AIN2      PBout(13)
#define  Out_AIN1      PBout(12)
#define  Out_BIN1      PBout(15)
#define  Out_BIN2      PBout(14)

#define  A  		   0
#define  B			   1
#define  dir1          0
#define  dir2          1

void Motor_gpio_Init(void);
void TIM1_PWM_Init(u8 arr, u8 psc);
void Circle_Control(int cir, int dir, u16 pwm_val);

#endif
