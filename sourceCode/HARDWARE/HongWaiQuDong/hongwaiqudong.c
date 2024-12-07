#include "hongwaiqudong.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

//初始化，包括设置时钟，输入输出模式，定时器，配置中断
void hongwai_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //GPIO初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //定时器4初始化结构体
	TIM_ICInitTypeDef TIM_ICInitStructure; //初始化输入捕获结构体
	NVIC_InitTypeDef NVIC_InitStructure; //中断初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//使能定时器4的时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9; //对PB9进行配置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD; //设置PB9为上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOB,GPIO_Pin_9); //拉高引脚的输出电平
	
	//初始化定时器4
	TIM_TimeBaseStructure.TIM_Period=10000; //时钟周期10ms，超过溢出
	TIM_TimeBaseStructure.TIM_Prescaler=(72-1); //预分频器，1M的计数频率，1us加1
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //设置时钟分割：TDTS=Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure); //根据指定参数初始化定时器4
	
	//输入捕获初始化
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_4; //选择输入端IC4映射到TI4上
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; //配置输入分频，不分频
	TIM_ICInitStructure.TIM_ICFilter=0x03; //配置输入滤波器，8个定时器时钟周期滤波
	TIM_ICInit(TIM4,&TIM_ICInitStructure); //初始化定时器输入捕获通道
	
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE); //允许更新中断，允许CC4IE捕获中断
}


//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 RmtSta=0;
u16 Dval; //下降沿时计数器的值
u32 RmtRec=0; //红外接受到的数据
u8 RmtCnt=0; //按键按下的次数

//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
	//发生了捕获中断
	if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
	{
		if(RDATA)//发生了上升沿捕获
		{
			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);
			TIM_SetCounter(TIM4,0); //清空定时器的值，清空以后再开始计数
			RmtSta|=0X10; //标记上升沿已经被捕获
		}
		else //下降沿捕获
		{
			Dval=TIM_GetCapture4(TIM4); //存放的是数据
			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //设置为上升沿捕获
			if(RmtSta&0X10) //已经捕获到了上升沿
			{
				if(RmtSta&0X80) //已经接受到了引导码
				{
					//开始判断接受到的数据
					if(Dval>300&&Dval<800) //捕获到560us的高电平，为逻辑0
					{
						RmtRec<<=1; //腾出1位来存放新获得的数据
						RmtRec|=0; //接受数据0
					}
					else if(Dval>1400&&Dval<1800) //捕获到1680us的高电平，为逻辑0
					{
						RmtRec<<=1; //腾出1位来存放新获得的数据
						RmtRec|=1; //接受数据0
					}
					else if(Dval>2200&&Dval<2600) //捕获到2.5ms的高电平，为逻辑0
					{
						RmtCnt++; //按键次数增加1
						RmtSta&=0XF0; //清空溢出计时器
					}
				}
				else if(Dval>4200&&Dval<4700) //接受到了4.5ms的高电平
				{
					RmtSta|=1<<7; //标记现在开始接受到了引导码
					RmtCnt=0; //清楚按键次数计数器
				}
			}
			RmtSta&=~(1<<4); //清楚上升沿已经被捕获标记
		}
	}

	
	//发生了更新中断
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80) //存在引导码
		{
			RmtSta&=~0x10; //取消上升沿已经被捕获标记
			if((RmtSta&0X0F)==0X00) RmtSta|=1<<6; //标记得到了一个按键的所有信息
			if((RmtSta&0X0F)<14) 
			{
				RmtSta++;
			}
			else
			{
				RmtSta&=~(1<<7); //清空引导码标识
				RmtSta&=0XF0; //清空计数器
			}
		}
	}	
	
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC4);
	
}

u8 hongwai_Scan(void)
	{
		u8 sta;
		u8 t1,t2;
		if(RmtSta&(1<<6)) //得到一个按键的所有信息
		{
			t1=RmtRec>>24; //得到地址码
			t2=(RmtRec>>16)&0xff; //得到地址反码
			if((t1==(u8)~t2)&&t1==REMOTE_ID)//检验遥控识别码(ID)及地址 
			{
				t1=RmtRec>>8;
				t2=RmtRec;
				if(t1==(u8)~t2) sta=t1; //键值正确
			}
			if((sta==0)||((RmtSta&0x80)==0)) //按键数据错误、遥控已经没有按下
			{
				RmtSta&=~(1<<6); //清除接收到有效按键标识
				RmtCnt=0;
			}
		}
		return sta;
	}
