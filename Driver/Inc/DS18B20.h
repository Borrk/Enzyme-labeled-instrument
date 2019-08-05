#ifndef __DS18_H__
#define __DS18_H__

#include "def.h"


#ifdef __cplusplus
extern "C" {
#endif


//初始化函数
void Init_DS18B20(void);
//读一个字节
unsigned char  ReadOneChar(void);
//写一个字节
void WriteOneChar(unsigned char dat);
//读取温度
double ReadTemperature(void);


#ifdef __cplusplus
}
#endif

#endif /*__DS18_H__*/
