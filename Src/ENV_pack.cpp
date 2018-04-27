/*
 * @Author: Zheng Qihang 
 * @Date: 2018-04-26 14:14:49 
 * @Last Modified by: Zheng Qihang
 * @Last Modified time: 2018-04-26 14:30:54
 */
#include "ENV_pack.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"
#include <cstring>
ENV_PACK::ENV_PACK(void)
{
    sprintf((char *)Header, "%s", "HJ"); //头赋�??
    Cnt = 0;
    Len = 41;
    Number = 0; //节点
    Fire=1;
    Smooth=0;
    Air_quilty=1;
    Gas=0;
    Dust=43;
    O2=25;
    Tmperture=27;
    Humidity=64;
    Lux=100;
    Pressure=102;
}


void ENV_PACK::Set_Fire(uint8_t x)
{
    Fire=x;
}
void ENV_PACK::Set_Smooth(uint8_t x)
{
    Smooth=x;
}
void ENV_PACK::Set_Air_quilty(uint8_t x)
{
    Air_quilty=x;
}
void ENV_PACK::Set_Gas(uint8_t x)
{
    Gas=x;
}
void ENV_PACK::Set_Dust(uint8_t x)
{
    Dust=x;
}
void ENV_PACK::Set_O2(uint8_t x)
{
    O2=x;
}
void ENV_PACK::Set_Tmperture(uint8_t x)
{
    Tmperture=x;
}
void ENV_PACK::Set_Humidity(uint8_t x)
{
    Humidity=x;
}
void ENV_PACK::Set_Lux(uint16_t x)
{
    Lux=x;
}
void ENV_PACK::Set_Pressure(uint16_t x)
{
    Pressure=x;
}


void ENV_PACK::data_update(uint8_t huoyan,uint8_t yanwu,uint8_t kongqi,uint8_t yehua,
                    uint8_t fencheng,uint8_t yangqi,uint8_t wendu,uint8_t shidu,
                    uint16_t guangqiang,uint16_t qiya)
{
        Fire=huoyan;
        Smooth=yanwu;
        Air_quilty=kongqi;
        Gas=yehua;
        Dust=fencheng;
        O2=yangqi;
        Tmperture=wendu;
        Humidity=shidu;
        Lux=guangqiang;
        Pressure=qiya;
}

void ENV_PACK::format(void)//格式化输�??
{
    Cnt= Cnt > 99 ? 0 : Cnt;
    memset(Env_buf,0,50);
    sprintf((char *)Env_buf,"%s%.2d%.2d%.3dH%1dY%1dK%1dB%1dF%.2dO%.2dW%.2dS%.2dG%.3dQ%.3d\r\n",
    Header,Cnt++,Len,Number,Fire,Smooth,Air_quilty,Gas,Dust,O2,Tmperture,Humidity,Lux,Pressure);
} 

void ENV_PACK::Send_Pack(void)//输出函数
{
     //USART4_printf()
    USART2_printf("%s",Env_buf);
} 


ENV_PACK env_pack;



