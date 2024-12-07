#include "wheel.h"
#include "pwm.h"

void car_forward(void)
{
	//控制前面俩个轮子正传
	TIM_SetCompare1(TIM3,5000);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,5000);
	TIM_SetCompare4(TIM3,0);
	
	
	//后面两个轮子正传
	TIM_SetCompare1(TIM2,5000);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,5000);
	TIM_SetCompare4(TIM2,0);
}

void car_backward(void)
{
	//控制前面俩个轮子反转
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,5000);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,5000);
	
	//后面两个轮子反转
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,5000);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,5000);
}

void car_right(void)
{
	//控制前左不转，前右转
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,5000);
	TIM_SetCompare4(TIM3,0);
	
	
	//控制后左不转，后右转
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,5000);
	TIM_SetCompare4(TIM2,0);
	
}


void car_left(void)
{
		//控制前右不转，前左转
	TIM_SetCompare1(TIM3,5000);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
	
	
	//控制后右不转，后左转
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
