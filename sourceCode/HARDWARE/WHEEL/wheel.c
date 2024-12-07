#include "wheel.h"
#include "pwm.h"

void car_forward(void)
{
	//����ǰ��������������
	TIM_SetCompare1(TIM3,5000);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,5000);
	TIM_SetCompare4(TIM3,0);
	
	
	//����������������
	TIM_SetCompare1(TIM2,5000);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,5000);
	TIM_SetCompare4(TIM2,0);
}

void car_backward(void)
{
	//����ǰ���������ӷ�ת
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,5000);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,5000);
	
	//�����������ӷ�ת
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,5000);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,5000);
}

void car_right(void)
{
	//����ǰ��ת��ǰ��ת
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,5000);
	TIM_SetCompare4(TIM3,0);
	
	
	//���ƺ���ת������ת
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,5000);
	TIM_SetCompare4(TIM2,0);
	
}


void car_left(void)
{
		//����ǰ�Ҳ�ת��ǰ��ת
	TIM_SetCompare1(TIM3,5000);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
	
	
	//���ƺ��Ҳ�ת������ת
	TIM_SetCompare1(TIM2,5000);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,0);
}

void stop(void)
{
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
	
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,0);
}
