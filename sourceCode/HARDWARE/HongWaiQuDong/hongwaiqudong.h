#ifndef __HONGWAIQUDONG_H
#define __HONGWAIQUDONG_H 
#include "sys.h"

#define RDATA PBin(9) //红外数据输入脚

#define REMOTE_ID 0//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
										//我们选用的遥控器识别码为0

extern u8 RmtCnt; //按键按下的次数

void hongwai_Init(void);    	//红外传感器接收头引脚初始化
u8 hongwai_Scan(void);	   

#endif
