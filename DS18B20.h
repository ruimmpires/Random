/******************************************************************************
----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
------------------------版权所有，仿冒必究！-----------------------------------
----------------1.开发环境:Arduino IDE-----------------------------------------
----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
----------------3.单片机使用晶振：16M------------------------------------------
----------------4.淘宝网址：Ilovemcu.taobao.com--------------------------------
----------------------------epic-mcu.taobao.com--------------------------------
----------------5.作者：神秘藏宝室---------------------------------------------
******************************************************************************/
#ifndef __DS18B20_H__
#define __DS18B20_H__


#include <Arduino.h>

class DS18B20
{
public:
  DS18B20(int pin);
  void DS18B20_Write_Byte( unsigned char dat);
  unsigned char DS18B20_Read_Byte();
  bool DS18B20_Init();
  float Get_temp() ;
private:
  int _DS18B20_DQ;
  unsigned char flag_temper;
};

#endif