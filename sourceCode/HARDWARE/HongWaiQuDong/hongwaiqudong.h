#ifndef __HONGWAIQUDONG_H
#define __HONGWAIQUDONG_H 
#include "sys.h"

#define RDATA PBin(9) //�������������

#define REMOTE_ID 0//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
										//����ѡ�õ�ң����ʶ����Ϊ0

extern u8 RmtCnt; //�������µĴ���

void hongwai_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 hongwai_Scan(void);	   

#endif
