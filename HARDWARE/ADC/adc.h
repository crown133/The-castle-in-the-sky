#ifndef _ADC_H_
#define _ADC_H_

#include "sys.h"

#define Check_ch 1
#define Cir1_ch  2
#define Cir2_ch  3


void Adc_Init(void);
void ADC_Filter(u16* adc_val);
void ADC_IT(u8 arr, u8 psc);
#endif
