#include "Adc1.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx.h"

ADC_HandleTypeDef hadc1;

void MY_ADC_Init(void)
{
hadc1.Instance = ADC1;
hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
hadc1.Init.Resolution = ADC_RESOLUTION_12B;
hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
hadc1.Init.ScanConvMode=DISABLE;
hadc1.Init.EOCSelection=DISABLE;                      //关闭EOC中断
hadc1.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
hadc1.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
hadc1.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
hadc1.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
hadc1.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
hadc1.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
HAL_ADC_Init(&hadc1);

}
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟

	GPIO_Initure.Pin = GPIO_PIN_0;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLDOWN;


    GPIO_Initure.Pin=GPIO_PIN_1;            //PA1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


uint16_t Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&hadc1);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        //返回最近一次ADC1规则组的转换结果
}



uint16_t get_ADCvalue(uint32_t ch)
{
    uint32_t adc_value=0;
 
  
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
    HAL_Delay(1);
    adc_value =Get_Adc(ch);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
    HAL_Delay(10);
    return adc_value;
    
}



