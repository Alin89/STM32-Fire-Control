/*
 * @Author: Zheng Qihang 
 * @Date: 2018-04-26 14:30:20 
 * @Last Modified by: Zheng Qihang
 * @Last Modified time: 2018-04-26 14:36:06
 */
#ifndef _ENV_PACK_H_
#define _ENV_PACK_H_
#include "stdint.h"

class ENV_PACK
{

  public:
    
    uint8_t Cnt;//数据包序号
    uint8_t Len;//数据包长度
    uint8_t Number;//节点编号
    uint8_t Fire;
    uint8_t Smooth;
    uint8_t Air_quilty;
    uint8_t Gas;
    uint8_t Dust;
    uint8_t O2;
    uint8_t Tmperture;
    uint8_t Humidity;
    uint16_t Lux;
    uint16_t Pressure;

    ENV_PACK(void);

    void Set_Fire(uint8_t x);
    void Set_Smooth(uint8_t x);
    void Set_Air_quilty(uint8_t x);
    void Set_Gas(uint8_t x);
    void Set_Dust(uint8_t x);
    void Set_O2(uint8_t x);
    void Set_Tmperture(uint8_t x);
    void Set_Humidity(uint8_t x);
    void Set_Lux(uint16_t x);
    void Set_Pressure(uint16_t x);


    void data_update(uint8_t huoyan,uint8_t yanwu,uint8_t kongqi,uint8_t yehua,
                    uint8_t fencheng,uint8_t yangqi,uint8_t wendu,uint8_t shidu,
                    uint16_t guangqiang,uint16_t qiya);

    void format(void);//格式化输出

    void Send_Pack(void);//输出函数

  private:

    uint8_t Header[2];//命令头
  
    uint8_t Env_buf[50]; //用于输出的环境数据包

};


extern ENV_PACK env_pack;

#endif


