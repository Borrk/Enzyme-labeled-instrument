#ifndef __DS18_H__
#define __DS18_H__

#include "def.h"


#ifdef __cplusplus
extern "C" {
#endif


//��ʼ������
void Init_DS18B20(void);
//��һ���ֽ�
unsigned char  ReadOneChar(void);
//дһ���ֽ�
void WriteOneChar(unsigned char dat);
//��ȡ�¶�
double ReadTemperature(void);


#ifdef __cplusplus
}
#endif

#endif /*__DS18_H__*/
