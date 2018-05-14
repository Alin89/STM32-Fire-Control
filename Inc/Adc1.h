#ifndef __Adc1_H
#define __Adc1_H
#include "stm32f4xx_hal.h"
#ifdef __cplusplus
 extern "C" {
#endif

extern ADC_HandleTypeDef  hadc1;

void MY_ADC_Init(void);
uint16_t Get_Adc(uint32_t ch);

uint16_t get_ADCvalue(uint32_t ch);







#ifdef __cplusplus
}
#endif
#endif


